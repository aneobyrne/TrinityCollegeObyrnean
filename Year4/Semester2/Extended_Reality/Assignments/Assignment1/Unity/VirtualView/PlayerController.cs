using UnityEngine;
// The RequireComponent attribute automatically adds a CharacterController as dependencies.
[RequireComponent(typeof(CharacterController))]
public class PlayerController : MonoBehaviour
{
    // The CharacterController used for simulating movement
    CharacterController characterController;
    // Movement parameters
    public float walkSpeed = 100.0f;
    public float rotateSpeed = 10.0f;
    void Start()
    {
        characterController = GetComponent<CharacterController>();
    }
    void Update()
    {
        Vector3 forward = transform.TransformDirection(Vector3.forward);
        Vector3 right = transform.TransformDirection(Vector3.right);
        // Character movement by WASD or arrow keys
        float curSpeedZ = walkSpeed * Input.GetAxis("Vertical");
        float curSpeedX = walkSpeed * Input.GetAxis("Horizontal");
        // The move direction is the linear combination of the forward and right
        Vector3 moveDirection = forward * curSpeedZ + right * curSpeedX;
        characterController.Move(moveDirection * Time.deltaTime);
        // Character view rotation by mouse
        if (Input.GetMouseButton(0))
        {
            // Note that vertical movement corresponds to rotation about X-axis.
            float angleX = Input.GetAxis("Mouse Y") * rotateSpeed;
            float angleY = Input.GetAxis("Mouse X") * rotateSpeed;
            transform.rotation *= Quaternion.AngleAxis(angleX, new Vector3(1, 0, 0)) *
            Quaternion.AngleAxis(angleY, new Vector3(0, 1, 0));
        }
    }
}