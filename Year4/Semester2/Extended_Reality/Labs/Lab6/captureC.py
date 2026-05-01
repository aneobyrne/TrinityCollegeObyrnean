# SAVE OUTPUT VIDEO FORMAT EXTENSION
import cv2
import time
import mediapipe as mp

# Prepare video capture from a file instead of webcam (UPDATE PATHS HERE)
#video_path = r"C:\Users\aneob\OneDrive\Documentos\TCD\Year4\Semester2\Extended Reality\Labs\Lab6\videos\HumanFacesVideo.mp4"  # <-- human faces video
video_path = r"C:\Users\aneob\OneDrive\Documentos\TCD\Year4\Semester2\Extended Reality\Labs\Lab6\videos\cartoonFaces.mp4" # <-- cartoon video 

cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    print("Error: Cannot open video file")
    exit()

# Video output (Change output file path HERE)
#output_path = r"C:\Users\aneob\OneDrive\Documentos\TCD\Year4\Semester2\Extended Reality\Labs\Lab6\videos\HumanFacesVideo_Output.mp4" # <-- human faces video
output_path = r"C:\Users\aneob\OneDrive\Documentos\TCD\Year4\Semester2\Extended Reality\Labs\Lab6\videos\CartoonFaces_Output.mp4"# <-- cartoon video 

# Get input video properties
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps_input = cap.get(cv2.CAP_PROP_FPS)

# Create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # for mp4
out = cv2.VideoWriter(output_path, fourcc, fps_input, (frame_width, frame_height))

# FPS setup
currentTime = 0
previousTime = 0

# MediaPipe setup 
mp_drawing = mp.solutions.drawing_utils
mp_face_mesh = mp.solutions.face_mesh
drawingSpec = mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=1, circle_radius=3)

face_mesh = mp_face_mesh.FaceMesh(
    max_num_faces=5,
    refine_landmarks=True,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)

# Processing loop 
while cap.isOpened():
    success, frame = cap.read()
    if not success:
        print("End of video or cannot read frame")
        break

    # Flip
    frame = cv2.flip(frame, 1)

    # Convert to RGB for MediaPipe
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = face_mesh.process(rgb_frame)

    # Draw landmarks for all detected faces
    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            mp_drawing.draw_landmarks(
                image=frame,
                landmark_list=face_landmarks,
                connections=mp_face_mesh.FACEMESH_TESSELATION,
                landmark_drawing_spec=drawingSpec,
                connection_drawing_spec=drawingSpec
            )

    # Calculate FPS
    currentTime = time.time()
    fps = 1 / (currentTime - previousTime) if previousTime != 0 else 0
    previousTime = currentTime

    # Display FPS on frame
    cv2.putText(frame, str(int(fps)) + " FPS", (10, 70),
                cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)

    # Show the frame
    cv2.imshow("Annotated Video", frame)

    # Write the frame to output video
    out.write(frame)

    # Exit early on Esc
    if cv2.waitKey(1) & 0xFF == 27:
        break

# Cleanup 
cap.release()
out.release()
cv2.destroyAllWindows()
print(f"Annotated video saved to: {output_path}")
