// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.



#include "RotateAutoGroup.h"
#include "driveAuto.h"
#include "RotatetoAngle.h"

AutoCommandGroup::AutoCommandGroup() {
	printf("\n In AutoCommandGroup::AutoCommandGroup() ");
	//DriveAuto(side, fow, rot, yaw, time);

	AddSequential(new RotatetoAngle(180, .5));

	AddSequential(new RotatetoAngle(0, .5));

	AddSequential(new driveAuto(0.0, -0.5, 0.0, 0.0, 2));

}