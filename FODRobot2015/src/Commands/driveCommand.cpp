// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "driveCommand.h"

driveCommand::driveCommand() {
	printf("\n In driveCommand::driveCommand()");

	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES

	Requires(Robot::driveBaseSub);

	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES


	// Define drive constants
	ANGLE_TOLERANCE = 2; // Tolerance in degrees on either side of set angle
	DRIVE_ANGLE = 0; // Only causing drive spins in this command - it doesn't need to know angle.
	DRIVE_X = 0; // used for translation magnitude to drive command - not used - stays 0
	DRIVE_Y = 0; // used for translation magnitude to drive command - not used - stays 0
	ROTATE_LOOP_CHECK = 5; //checks for iterations through loop. Change later depending on robot
	SetAngleScaled = 0;
	SetAngle = 0;
	SetSpeed = 0;
	RotCmd = 0;
	Button2Pressed = false;
	MaxScalingSpeed = 1;
	DegreesToSetPointAbs = 0;
	DriverRotateAxisOverride = 0;
	SpinDirection = 1;
	AutoRotDone = true;
	DegreesToSetPoint = 0;
	RotateAxis = 0;
	TooFarCCW = false;
	TooFarCW = false;
	AutoRotCmd = 0;
	SetInitAngle = 0;
	YAxis = 0;
	XAxis = 0;
	IMU_Scaled = 0;
	IMU_Yaw = 0;
	TimesThroughLoop = 0;
}

// Called just before this Command runs the first time
void driveCommand::Initialize() {
	printf("\n In driveCommand::Initialize");

	Button2Pressed = false; //B button on xbox 360 controller used to start AutoRotate

	SetSpeed = 0; // Current spin speed for piece wise linear angle from set point distance
	MaxScalingSpeed = 1; // Scales SetSpeed by overall scaling factor (used to slow down whole spin)

	SetAngle = 0; // Saves the commanded go-to angle functional parameter passed in.
	SetInitAngle = 0; // Initial Set Angle for Rotation

	//Default values for drive command - joystick overrides these
	XAxis = 0;
	YAxis = 0;
	RotateAxis = 0;

	DriverRotateAxisOverride = 0; //Driver override AutoRotation value
	
	IMU_Yaw = 0;  //Yaw value from IMU and scaled value to eliminate
	IMU_Scaled = 0; // Translated to a non-wrapping scale (-180 to 180 -> 820 to 1180)
	SetAngleScaled = 0; // Converted value of the Commanded Set Angle to linear scale.

	SpinDirection = 1;  // Sets pin direction: 1=Clockwise from robot top, -1=CCW
	RotCmd = 0; //Calculated rotation speed to command

	//Assume we don't need the robot to spin until proven via measurement.
	TooFarCW = false; // Robot is further CW than set point: needs to spin CCW
	TooFarCCW = false; //Robot is further CCW than set point: needs to spin CW

	DegreesToSetPoint = 0; //Number of degrees from current angle to set point
	DegreesToSetPointAbs = 0; //absolute value of DegreesToSetPoint

	//Persistence of measurements of current angle to determine if system has stopped moving, overshooting
	TimesThroughLoop = 0;

	AutoRotCmd = 0; // Rotate command from AutoRotate calculator

	AutoRotDone = true; //Rotate command is done


	printf("\n Done driveCommand::Initialize()");
}

// Called repeatedly when this Command is scheduled to run
void driveCommand::Execute() {

	printf("\n In driveCommand::Execute(");

	//Set the value back from the Autorotate command to zero for each passd through the loop
	AutoRotCmd = 0;

	//Button press indicates start AutoROtate command
	Button2Pressed = Robot::oi->getJoystick1()->GetRawButton(2);


	//Start if the button is pressed, or if we aren't done from previous conditions
	if(((Button2Pressed == true) && (AutoRotDone == true)) || (AutoRotDone == false))
	{
		//Rotate to Set Angle at Max Speed (1)
		AutoRotCmd = RotateToAngleDrive(SetAngle, MaxScalingSpeed);
	}


	//Read current robot orientation angle measured from starting position=0 degrees
	IMU_Yaw = Robot::driveBaseSub->pRobot_IMU->GetYaw();
	SmartDashboard::PutNumber("Angle of Robot", IMU_Yaw);


	//Get Joystick Values from Driver
	XAxis = Robot::oi->getJoystick1()->GetRawAxis(0);
	YAxis = Robot::oi->getJoystick1()->GetRawAxis(1);
	RotateAxis = Robot::oi->getJoystick1()->GetRawAxis(4);

	//Sets up deadbands (this gets rid of the robot moving very slowly
	//even when you are not touching the joystick. Because the joystick
	//is never really at zero, (for example 0.0389583), this checks if
	//any joystick is in the range of -0.20 to 0.20 and if it is sets it
	//to zero, if not, sends the current value to the MechDrive function.

	//Also squares the values which increases sensitivity, and (mostly) blends out
	//the discontinuity you would have with just the dead band (there's no step from 0 to .2,
	//the lowest value is 0 to .04 now.

	if(XAxis > -0.20 && XAxis < 0.20)
	{
		XAxis = 0;
	}
	else
	{
		XAxis = XAxis * fabs(XAxis);
	}



	if(YAxis > -0.20 && YAxis < 0.20)
	{
		YAxis = 0;
	}
	else
	{
		YAxis = YAxis * fabs(YAxis);
	}


	
	if(RotateAxis > -0.20 && RotateAxis < 0.20)
	{
		RotateAxis = 0;
	}
	else
	{
		RotateAxis = RotateAxis * fabs(RotateAxis);
	}

	SmartDashboard::PutNumber("JoyX", XAxis);
	SmartDashboard::PutNumber("JoyY", YAxis);
	SmartDashboard::PutNumber("JoyRot", RotateAxis);


	//Auto Rotation
	if(RotateAxis == 0)
	{
		Robot::driveBaseSub->MechDrive(XAxis,YAxis,AutoRotCmd,IMU_Yaw);

		printf("\n Command Drive with AutoRotate");
	}
	//Driver rotation
	else
	{
		Robot::driveBaseSub->MechDrive(XAxis,YAxis,RotateAxis,IMU_Yaw);
		TimesThroughLoop = 0;  //reset the loop/overshoot counter any time driver overrides
		AutoRotDone = true;

		printf("\n Command Drive with Driver / Joystick Rotation (If Any)");
	}


	printf("\n Leaving driveCommand::Execute()");


}

float driveCommand::RotateToAngleDrive(float Angle, float Speed) {

	printf("\n In driveCommand::RotateToAngleDrive");

	AutoRotDone = false; // Just started - can't be done yet

	//passes in values
	SetInitAngle = Angle;
	MaxScalingSpeed = Speed;

	DriverRotateAxisOverride = Robot::oi->getJoystick1()->GetRawAxis(4); //get joystick for override

	//Assume we don't need the robot to spin until proven via measurement.
	TooFarCW = false;
	TooFarCCW = false;

	//Default to no rotation commanded
	RotCmd = 0;

	//Read the actual Robot Angle from IMU
	IMU_Yaw = Robot::driveBaseSub->pRobot_IMU->GetYaw();

	//Scale the IMU reading
	IMU_Scaled = IMU_Yaw + 1000;

	//Scale Set Angle
	SetAngleScaled = SetInitAngle + 1000;
	printf("\n(DriveRotateLoop) Scaled IMU %3.2f  Scaled Set Angle %3.2f\n", IMU_Scaled, SetAngleScaled);

	//set spin direction and degrees to rotate to
	if(IMU_Scaled > (SetAngleScaled + ANGLE_TOLERANCE)) {
		TooFarCW = true;
		SpinDirection = -1;
		DegreesToSetPoint = IMU_Scaled - SetAngleScaled;
	}

	//set spin direction and degrees to rotate to
	if (IMU_Scaled < (SetAngleScaled - ANGLE_TOLERANCE)) {
		TooFarCCW = true;
		SpinDirection = 1;
		DegreesToSetPoint = SetAngleScaled - IMU_Scaled;
	}


	//Only start spinning if we need to.
	if (TooFarCW || TooFarCCW) {
		DegreesToSetPointAbs = fabs(DegreesToSetPoint);

		//scale speeds based on degrees. Why not an exponential function? :(
		if(DegreesToSetPointAbs <= 180)
		{
			SetSpeed = 1;
		}
		if(DegreesToSetPointAbs < 90)
		{
			SetSpeed = 0.4;
		}
		if(DegreesToSetPointAbs < 60)
		{
			SetSpeed = 0.3;
		}
		if(DegreesToSetPointAbs < 30)
		{
			SetSpeed = 0.2;
		}

		// Convert from absolute angle back to direction based
		RotCmd = SpinDirection * SetSpeed;
		// Multiply in a master speed scaling factor that was passed in if needed
		RotCmd = RotCmd * MaxScalingSpeed;

		//We need to spin, so set counter to 1 - it shouldn't increment until it has previously spun then come to rest
		TimesThroughLoop = 1;

		printf("\n(DriveRotateLoop) SpinCW = %d     SpinCCW = %d     SpinDir = %d     RotCmd = %3.2f\n", TooFarCCW, TooFarCW, SpinDirection, RotCmd);

	}


	// If we didn't need to spin, we can end the command now.
	else {
		printf("\n(DriveRotateLoop) Didn't need to spin.");
		// either the robot has stabilized for so long after rotating or it never rotated to begin with
		if(TimesThroughLoop == ROTATE_LOOP_CHECK || TimesThroughLoop == 0)
		{
			AutoRotDone = true;
			// reset the loop counter now that the rotation is complete and stable
			TimesThroughLoop = 0;
			RotCmd = 0;
			printf("\n(DriveRotateLoop) Spinning Can Stop Now ");
		}

		//robot is within tolerance but hasn't yet been stable for the timeout
		TimesThroughLoop++;

	}

	printf("\n Leaving driveCommand::RotateToAngleDrive");
	return RotCmd;
}

// Make this return true when this Command no longer needs to run execute()
bool driveCommand::IsFinished() {
	printf("\n In driveCommand::IsFinished()");
	return false;
}

// Called once after isFinished returns true
void driveCommand::End() {
	printf("\n In driveCommand::End()");
	Robot::driveBaseSub->MechDrive(0,0,0,0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void driveCommand::Interrupted() {
	printf("\n In driveCommand::Interrupted()");
	Robot::driveBaseSub->MechDrive(0,0,0,0);
}