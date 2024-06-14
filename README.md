# Gimbal_Control
Open source control system for Gimbal Unit based on the paper "3-D printable open source dual axis gimbal system for optoelectronic measurements"

The main "Gimbal" File is the fastest but allows for some wobble during movement.
"Gimbal_No_Wobble" slows down the gimbal to 1 step per cycle but eliminates most if not all of the wobble.

"Gimbal_Dual_Controller" allows for controlling the gimbal in parallel with multiple controllers. Both controllers must be running the same code and should be set up the same, except the y pin of the joystick is tied to the second controller and not the first.
