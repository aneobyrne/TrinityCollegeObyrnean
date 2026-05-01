# LIBRARIES
import mediapipe as mp # Landmark detection
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

import numpy as np
import cv2 # Camera access
import time
import os

# Create a MediaPipe HandLandmarker detector
model_path = os.path.join(os.path.dirname(__file__), 'assignment2', 'hand_landmarker.task') # <-- Update PATH here!
base_options = python.BaseOptions(model_asset_path=model_path)
options = vision.HandLandmarkerOptions(base_options=base_options, num_hands=2) # Support 2 hands on screen
detector = vision.HandLandmarker.create_from_options(options)

'''
TASK 1: Implement the hand landmark prediction using MediaPipe's HandLandmarker
'''
# Run MediaPipe model on an image
def predict(frame): 
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=frame)
    detection_result = detector.detect(mp_image)
    return detection_result # Result contains both 2D and 3D coordinates (21 landmark points for each hand)

# Helper function to draw the detected 2D landmarks on an image
def draw_landmarks_on_image(image, detection_result): 
    if not detection_result or not detection_result.hand_landmarks:
        return image 
    
    # Hand connection indices (0-indexed)
    HAND_CONNECTIONS = [
        (0, 1), (1, 2), (2, 3), (3, 4),  # Thumb
        (0, 5), (5, 6), (6, 7), (7, 8),  # Index
        (0, 9), (9, 10), (10, 11), (11, 12),  # Middle
        (0, 13), (13, 14), (14, 15), (15, 16),  # Ring
        (0, 17), (17, 18), (18, 19), (19, 20),  # Pinky
        (5, 9), (9, 13), (13, 17)  # Palm connections
    ]
    
    hand_landmarks_list = detection_result.hand_landmarks
    frame_height, frame_width = image.shape[:2]
    
    # Draw connections
    for hand_landmarks in hand_landmarks_list:
        for start_idx, end_idx in HAND_CONNECTIONS:
            start = hand_landmarks[start_idx]
            end = hand_landmarks[end_idx]
            start_pos = (int(start.x * frame_width), int(start.y * frame_height))
            end_pos = (int(end.x * frame_width), int(end.y * frame_height))
            cv2.line(image, start_pos, end_pos, (0, 255, 0), 2)
        
        # Draw landmarks (convert to pixel coordinates)
        for landmark in hand_landmarks:
            x = int(landmark.x * frame_width)
            y = int(landmark.y * frame_height)
            cv2.circle(image, (x, y), 5, (0, 0, 255), -1)
    
    return image

# Create a 3x3 matrix to represent the camera intrinsic properties
def get_camera_matrix(frame_width, frame_height, scale=1.0): 
    focal_length = frame_width * scale # As we do not know exactly the focal length, we estimate it by a scale of the image size
    aspect_ratio = 1.0 # Reflects ratio in the physical pixel size
    center = (frame_width / 2.0, frame_height / 2.0) # Center of projection. We simply take the image center
    
    # 3x3 intrinsic matrix
    camera_matrix = np.array(
        [[focal_length, 0, center[0]],
        [0, focal_length, center[1]],
        [0, 0, 1]], dtype = "double"
    )
    return camera_matrix

# Compute the vertical field of view from focal length for OpenGL rendering
def get_fov_y(camera_matrix, frame_height): 
    focal_length_y = camera_matrix[1][1]
    fov_y = np.rad2deg(2 * np.arctan2(frame_height, 2 * focal_length_y))
    return fov_y

# Convert the rotation vector and translation vector to a 4x4 matrix
def get_matrix44(rvec, tvec): 
	rvec = np.asarray(rvec)
	tvec = np.asarray(tvec)
	T = np.eye(4)
	R, jac = cv2.Rodrigues(rvec)
	T[:3, :3] = R
	T[:3, 3] = tvec
	return T

# Fins 3D pose of the hand by solving the Perspective-n-Point problem
def solvepnp(model_landmarks_list, image_landmarks_list, 
            camera_matrix, frame_width, frame_height): 
    
    if not model_landmarks_list: # If no hands detected, return an empty list
        return []

    if len(model_landmarks_list) != len(image_landmarks_list): # Ensure both lists contain the same number of hands
        return []
    
    world_landmarks_list = []
    
    # Loop through each detected hand and solvePnP for each one
    for i in range(len(model_landmarks_list)):

        model_landmarks = model_landmarks_list[i]
        image_landmarks = image_landmarks_list[i]
        
        # N x 3 matrix
        model_points = np.float32([[l.x, l.y, l.z] for l in model_landmarks])
        image_points = np.float32([[l.x * frame_width, l.y * frame_height] for l in image_landmarks])
        
        world_points = np.copy(model_points)

        '''
        TASK 2: Call OpenCV's solvePnP function to find rotation and translation
        '''
        if len(model_points) >= 4: # Needs at leas 4 points to solve PnP (we have 21)
            # Solve the pose
            _, rvec, tvec = cv2.solvePnP(model_points, image_points, camera_matrix, None, flags=cv2.SOLVEPNP_ITERATIVE)
            # Transform model points to camera/world space
            R, _ = cv2.Rodrigues(rvec)
            world_points = (R @ model_points.T).T + tvec.T
        
        # Store all 3D landmarks
        world_landmarks_list.append(world_points)
    
    return world_landmarks_list # List of 21x3 matrixes representing hand landmarks in absolute world space (3D)

# Function that checks how close the 3D landmarks are to the original 2D 
def reproject(world_landmarks_list, image_landmarks_list, 
              camera_matrix, frame_width, frame_height): 

    # Perform a perspective projection of 3D points onto the image plane and return the projected points
    reprojection_points_list = []
    reprojection_error = 0.0

    for (world_landmarks, image_landmarks) in zip(world_landmarks_list, image_landmarks_list):
        
        # Perspective projection by multiplying with the intrinsic matrix
        output = world_landmarks.dot(camera_matrix.T)
        
        # Perspective division
        output[:, 0] /= output[:, 2]
        output[:, 1] /= output[:, 2]
        
        # Store the results into a list for visualization later
        reprojection_points_list.append(output[:, :2])
    
        # Calculate the reprojection error, per point
        image_points = np.float32([[l.x * frame_width, l.y * frame_height] for l in image_landmarks])
        reprojection_error += np.linalg.norm(output[:, :2] - image_points) / len(output) / len(world_landmarks_list)
    
    return reprojection_error, reprojection_points_list


# MAIN FUNCTION
if __name__ == '__main__':
    # (0) in VideoCapture is used to connect to your computer's default camera
    capture = cv2.VideoCapture(0)
    
    # Initializing current time and precious time for calculating the FPS
    previousTime = 0
    currentTime = 0
    
    while capture.isOpened():
        # Capture frame by frame
        ret, frame = capture.read() 

        # Safety check if camera frame fails (DEBUG ADDITION)
        if not ret: 
            break
    
        # Resizing the frame for better view
        aspect_ratio = frame.shape[1] / frame.shape[0]
        frame = cv2.resize(frame, (int(720 * aspect_ratio), 720))
        frame = cv2.flip(frame, 1)

        # Converting the from BGR to RGB
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
        # Making predictions (detect the hands)
        detection_result = predict(frame)
    
        # Visualize 2D landmarks (hand skeleton)
        frame = draw_landmarks_on_image(frame, detection_result)
        
        '''
        TASK 2: SolvePnP, and visualize the reprojected landmarks. The reprojected points should be close enought to the 2D landmarks
        '''
        # Variables for 3D processing
        world_landmarks_list = []
        reprojection_points_list = []
        avg_reprojection_error = 0.0

        # Check if hands are detected and if it contains landmarks, solve
        if detection_result and detection_result.hand_landmarks:

            model_landmarks_list = detection_result.hand_world_landmarks
            image_landmarks_list = detection_result.hand_landmarks

            camera_matrix = get_camera_matrix(frame.shape[1], frame.shape[0])
            world_landmarks_list = solvepnp(model_landmarks_list, image_landmarks_list, camera_matrix, frame.shape[1], frame.shape[0])
            avg_reprojection_error, reprojection_points_list = reproject(world_landmarks_list, image_landmarks_list, camera_matrix, frame.shape[1], frame.shape[0])
        
        # Visualize the reprojected points
        for hand_landmarks in reprojection_points_list:
            for l in hand_landmarks:
                cv2.circle(frame, (int(l[0]), int(l[1])), 3, (0, 0, 255), 2)
        
        # Calculate the FPS
        currentTime = time.time()
        fps = 1 / (currentTime - previousTime)
        previousTime = currentTime
        
        # Display FPS and average reprojection error on the image
        cv2.putText(frame, f"{int(fps)} FPS", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.putText(frame, f"Reprojection Error: {avg_reprojection_error:.2f}", (10, 80), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    
        # Display the resulting image
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        cv2.imshow("", frame)
    
        # Enter key 'q' to break the loop
        if cv2.waitKey(5) & 0xFF == 27:
            break
    
    # When all the process is done. Release the capture and destroy all windows
    capture.release()
    cv2.destroyAllWindows()