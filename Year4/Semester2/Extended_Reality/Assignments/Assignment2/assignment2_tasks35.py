# LIBRARIES
import moderngl
import moderngl_window as mglw
from pyrr import Matrix44

import cv2
import numpy as np
import os
from array import array

# Import function from other file containing Task 1 & 2
from assignment2_tasks12 import predict, get_camera_matrix, get_fov_y, solvepnp 

# AR Window application class (main control)
class CameraAR(mglw.WindowConfig):
    # OpenGL settings
    gl_version = (3, 3)
    title = "CameraAR"
    resource_dir = os.path.normpath(os.path.join(os.path.dirname(__file__), 'assignment2', 'data')) 
    previousTime = 0
    currentTime = 0
    
    # Initialization function
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        # Enable program point size for variable point sizes
        self.ctx.enable(moderngl.PROGRAM_POINT_SIZE)

        # Shader for rendering 3D objects
        self.prog3d = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_position;
                in vec3 in_normal;
                in vec2 in_texcoord_0;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec2 v_text;

                void main() {
                    gl_Position = Mvp * vec4(in_position, 1.0);
                    v_vert = in_position;
                    v_norm = in_normal;
                    v_text = in_texcoord_0;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform vec3 Color;
                uniform vec3 Light;
                uniform sampler2D Texture;
                uniform bool withTexture;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    if (withTexture) {
                        f_color = vec4(Color * texture(Texture, v_text).rgb * lum, 1.0);
                    } else {
                        f_color = vec4(Color * lum, 1.0);
                    }
                }
            ''',
        )
        self.mvp = self.prog3d['Mvp']
        self.light = self.prog3d['Light']
        self.color = self.prog3d['Color']
        self.withTexture = self.prog3d['withTexture']

        # Load the 3D virtual object, and the marker for hand landmarks
        self.scene_cube = self.load_scene('crate.obj')
        self.scene_marker = self.load_scene('marker.obj')

        # Extract the VAOs from the scene
        self.vao_cube = self.scene_cube.root_nodes[0].mesh.vao.instance(self.prog3d)
        self.vao_marker = self.scene_marker.root_nodes[0].mesh.vao.instance(self.prog3d)

        # Texture of the cube
        self.texture = self.load_texture_2d('crate.png')
        
        # Define the initial position of the virtual object
        self.object_pos = np.array([0.0, 0.0, -30.0]) 

        # For smoothing 3D hand landmarks
        self.prev_smoothed_world_landmarks = None
        self.smoothing_alpha = 0.7  # Higher = smoother, try 0.6-0.8
        

        """
        TASK 3: Add support to render a rectangle of window size
        """
        # Shader program for rendering OpenCV video frame as a background texture
        self.prog_bg = self.ctx.program(
            vertex_shader='''
                #version 330
                in vec2 in_pos;
                in vec2 in_tex;
                out vec2 v_tex;

                void main() {
                    gl_Position = vec4(in_pos, 0.0, 1.0);
                    v_tex = in_tex;
                }
            ''',
            fragment_shader='''
                #version 330
                uniform sampler2D Texture;
                in vec2 v_tex;
                out vec4 f_color;

                void main() {
                    f_color = texture(Texture, v_tex);
                }
            '''
        )

        # Fullscreen rectangle covering the whole window
        vertices = np.array([
            -1.0, -1.0, 0.0, 1.0,
             1.0, -1.0, 1.0, 1.0,
            -1.0,  1.0, 0.0, 0.0,
             1.0,  1.0, 1.0, 0.0,
        ], dtype='f4')

        self.vbo_bg = self.ctx.buffer(vertices.tobytes())

        self.vao_bg = self.ctx.vertex_array(
            self.prog_bg,
            [(self.vbo_bg, '2f 2f', 'in_pos', 'in_tex')]
        )

        # Placeholder texture for the camera frame
        self.texture_bg = None

        # Shader for rendering 2D points (landmarks) in OpenGL
        self.prog_point = self.ctx.program(
            vertex_shader='''
                #version 330
                uniform mat4 Mvp;
                uniform float PointSize;
                in vec3 in_position;
                void main() {
                    gl_Position = Mvp * vec4(in_position, 1.0);
                    gl_PointSize = PointSize;  
                }
            ''',
            fragment_shader='''
                #version 330
                uniform vec3 Color;
                out vec4 f_color;
                void main() {
                    f_color = vec4(Color, 1.0);
                }
            '''
        )
        self.mvp_point = self.prog_point['Mvp']
        self.color_point = self.prog_point['Color']

        # Create a simple point cloud VAO for rendering landmarks as points
        landmark_vertices = np.zeros((2, 3), dtype='f4')  # Space for 2 points (for lines)
        self.vbo_landmark = self.ctx.buffer(landmark_vertices.tobytes())
        self.vao_landmark_points = self.ctx.vertex_array(
            self.prog_point,
            [(self.vbo_landmark, '3f', 'in_position')]
        )

        # Start OpenCV camera 
        self.capture = cv2.VideoCapture(0)
        
        # Get a frame to set the window size and aspect ratio
        ret, frame = self.capture.read() 
        self.aspect_ratio = float(frame.shape[1]) / frame.shape[0]
        self.window_size = (int(720.0 * self.aspect_ratio), 720)

    """
    TASK 5: Helper functions for each action
    """
    # Detects pinch gesture based on hand landmarks (if thumb and index finger tips are close)
    def is_pinch(self, hand_landmarks, threshold=4.5):
        thumb_tip = hand_landmarks[4]
        index_tip = hand_landmarks[8]
        distance = np.linalg.norm(thumb_tip - index_tip)
        return distance < threshold

    # Detects if index finger tip is close to the cube
    def is_touching_cube(self, hand_landmarks, cube_pos, hit_radius=5.0):
        index_tip = hand_landmarks[8]
        distance = np.linalg.norm(index_tip - cube_pos)
        return distance < hit_radius

    # Move cube to the midpoint between thumb and index finger tips when pinched and close are true
    def move_cube(self, hand_landmarks):
        thumb_tip = hand_landmarks[4]
        index_tip = hand_landmarks[8]
        return (thumb_tip + index_tip) / 2 # Move cube to midpoint

    # Smooth movement function for moving the cube
    def lerp(self, a, b, t=0.2):
        return a * (1-t) + b * t

  
    # MAIN RENDERING FUNCTION
    def on_render(self, time: float, frame_time: float):
        self.ctx.clear(1.0, 1.0, 1.0)
        self.ctx.enable(moderngl.CULL_FACE)
        detection_result = None 

        """
        TASK 3: Get OpenCV video frame, display in OpenGL
        """
        ret, frame = self.capture.read()
        if not ret:
            return
        frame = cv2.flip(frame, 1)

        # DEBUG INFORMATION
        if not hasattr(self, 'frame_count'):
            self.frame_count = 0
            self.fps_time = time
            self.fps = 0
        self.frame_count += 1

        # Compute FPS
        if time - self.fps_time > 1.0:
            self.fps = self.frame_count / (time - self.fps_time)
            self.frame_count = 0
            self.fps_time = time

        # Use last computed values for overlay
        fps_val = self.fps if hasattr(self, 'fps') else 0
        reproj_val = getattr(self, 'last_reproj_error', 0)
        pinch_val = getattr(self, 'last_pinch', False)
        hit_val = getattr(self, 'last_hit', False)
        cv2.putText(frame, f"FPS: {fps_val:.1f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,255,0), 1)
        cv2.putText(frame, f"Reproj Error: {reproj_val:.2f}px", (10, 55), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,255,0), 1)
        cv2.putText(frame, f"Pinch: {pinch_val}  Hit: {hit_val}", (10, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0,128,255), 1)


        # Convert BGR (OpenCV) to RGB (OpenGL)
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Create texture
        if self.texture_bg is None:
            h, w, _ = frame_rgb.shape
            self.texture_bg = self.ctx.texture((w, h), 3)
            self.texture_bg.build_mipmaps()

        # Update texture with camera frame
        self.texture_bg.write(frame_rgb.tobytes())

        # Render fullscreen rectangle with camera texture
        self.ctx.disable(moderngl.DEPTH_TEST)
        self.texture_bg.use()
        self.vao_bg.render(moderngl.TRIANGLE_STRIP)


        """
        TASK 4: Perform hand landmark prediction, and solve PnP to get world landmarks list
        """
        # Predict landmarks using MediaPipe
        detection_result = predict(frame_rgb)
        world_landmarks_list = []

        # Compute world landmarks using solvePnP
        if detection_result and detection_result.hand_landmarks:
            model_landmarks_list = detection_result.hand_world_landmarks
            image_landmarks_list = detection_result.hand_landmarks
            camera_matrix = get_camera_matrix(frame.shape[1], frame.shape[0])

            # SolvePnP to get world landmarks
            world_landmarks_list = solvepnp(
                model_landmarks_list,
                image_landmarks_list,
                camera_matrix,
                frame.shape[1],
                frame.shape[0]
            )

            # Compute and print average reprojection error (in pixels)
            from assignment2_tasks12 import reproject
            if world_landmarks_list:
                reproj_error, _ = reproject(
                    world_landmarks_list,
                    image_landmarks_list,
                    camera_matrix,
                    frame.shape[1],
                    frame.shape[0]
                )
                print(f"Reprojection error (avg, px): {reproj_error:.2f}")
                self.last_reproj_error = reproj_error

            # Convert meters to centimeters and OpenCV to OpenGL coordinates
            for i in range(len(world_landmarks_list)):
                world_landmarks_list[i] *= 100
                world_landmarks_list[i][:, 0] *= -1 

            # Smoothing of landmarkis to reduce jitter
            if world_landmarks_list:
                if self.prev_smoothed_world_landmarks is None or len(self.prev_smoothed_world_landmarks) != len(world_landmarks_list):
                    self.prev_smoothed_world_landmarks = [np.copy(lm) for lm in world_landmarks_list]
                else:
                    alpha = self.smoothing_alpha
                    for i, (prev, curr) in enumerate(zip(self.prev_smoothed_world_landmarks, world_landmarks_list)):
                        self.prev_smoothed_world_landmarks[i] = alpha * prev + (1 - alpha) * curr
                world_landmarks_list = [np.copy(lm) for lm in self.prev_smoothed_world_landmarks]
            else:
                self.prev_smoothed_world_landmarks = None


        """
        TASK 5: Detect gestures and interact with the cube
        """
        grabbed = False 

        # For each detected hand, check for pinch and hit gestures
        if world_landmarks_list:  
            for hand_landmarks in world_landmarks_list:
                pinch_dist = np.linalg.norm(hand_landmarks[4] - hand_landmarks[8])
                hit_dist = np.linalg.norm(hand_landmarks[8] - self.object_pos)

                pinch = self.is_pinch(hand_landmarks)
                hit = self.is_touching_cube(hand_landmarks, self.object_pos)

                # Debug info printing in terminal
                print(f"Pinch dist: {pinch_dist:.2f} cm | Hit dist: {hit_dist:.2f} cm | Pinch: {pinch} | Hit: {hit}")
                
                # Store for overlay
                self.last_pinch = pinch
                self.last_hit = hit

                # If both pinch and hit actions performed, move cube with hand
                if pinch and hit:
                    grabbed = True
                    self.object_pos = self.lerp(self.object_pos, self.move_cube(hand_landmarks))


        """
        TASK 4: Render the markers.
        """
        # Set projection matrix from camera intrinsics
        if detection_result and detection_result.hand_landmarks:
            fov_y = get_fov_y(camera_matrix, frame.shape[0])
        else:
            fov_y = 45
        proj = Matrix44.perspective_projection(fov_y, self.aspect_ratio, 0.1, 1000)

        # Translate the object to its position 
        translate = Matrix44.from_translation(self.object_pos)
        
        # Add a bit of random rotation just to be dynamic
        rotate = Matrix44.from_y_rotation(np.sin(time) * 0.5 + 0.2)
        
        # Scale the object up for easy viewing
        scale = Matrix44.from_scale((4, 4, 4)) 
        
        mvp = proj * translate * rotate * scale
        self.color.value = (1.0, 1.0, 1.0)
        if grabbed: # A bit of feedback when the object is grabbed
            self.color.value = (1.0, 0.0, 0.0)
        self.light.value = (10, 10, 10)
        self.mvp.write(mvp.astype('f4'))
        self.withTexture.value = True
        
        # Render the object (cube)
        self.texture.use()
        self.vao_cube.render()
        
        # Render the 3D hand landmarks as green plus signs 
        for hand_landmarks in world_landmarks_list:
            for landmark in hand_landmarks:
                for axis in range(2): 
                    offset = np.zeros(3, dtype='f4')
                    offset[axis] = 0.2  

                    p1 = landmark - offset
                    p2 = landmark + offset

                    line = np.stack([p1, p2])
                    self.vbo_landmark.write(line.astype('f4').tobytes())
                    mvp_marker = proj * Matrix44.identity()

                    self.mvp_point.write(mvp_marker.astype('f4'))
                    self.color_point.value = (0.0, 1.0, 0.0)  
                    self.prog_point['PointSize'].value = 3.0
                    self.ctx.line_width = 20.0  
                    self.vao_landmark_points.render(moderngl.LINES)

        # Render 2D hand landmarks as red dots 
        self.ctx.disable(moderngl.DEPTH_TEST)
        if detection_result and detection_result.hand_landmarks:
            for hand_landmarks in detection_result.hand_landmarks:
                for idx, lm in enumerate(hand_landmarks):
                    x = lm.x * frame.shape[1]
                    y = lm.y * frame.shape[0]
                    
                    ndc_x = (x / frame.shape[1]) * 2.0 - 1.0
                    ndc_y = 1.0 - (y / frame.shape[0]) * 2.0

                    pos = np.array([ndc_x, ndc_y, 0.01], dtype='f4')
                    self.vbo_landmark.orphan()
                    self.vbo_landmark.write(pos.astype('f4').tobytes())
                    self.mvp_point.write(Matrix44.identity().astype('f4'))
                    self.color_point.value = (1.0, 0.0, 0.0)  
                    self.prog_point['PointSize'].value = 8.0  
                    self.vao_landmark_points.render(moderngl.POINTS)
        self.ctx.enable(moderngl.DEPTH_TEST)

        # FPS display in terminal
        if not hasattr(self, 'frame_count'):
            self.frame_count = 0
            self.fps_time = time
            self.fps = 0
        self.frame_count += 1
        if time - self.fps_time > 1.0:
            self.fps = self.frame_count / (time - self.fps_time)
            print(f"FPS: {self.fps:.1f}")
            self.frame_count = 0
            self.fps_time = time

# MAIN FUNCTION
if __name__ == '__main__':
    CameraAR.run()