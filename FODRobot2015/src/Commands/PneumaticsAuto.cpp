#include <Commands/PneumaticsAuto.h>

PneumaticsAuto::PneumaticsAuto(bool left, bool right, bool oc)
{
	// Use Requires() here to declare subsystem dependencies
	Requires(Robot::gripperArm);
	time_timer = 0;
	time_run = PNEUMATIC_DELAY;
	timer = new Timer();
	PneumaticsDone = false;
	leftArm = left;
	rightArm = right;
	open_close = oc;
	//printf("PneumaticsAuto const\n");
}

// Called just before this Command runs the first time
void PneumaticsAuto::Initialize()
{
	//printf("PneumaticsAuto init\n");
	time_timer = 0;
	time_run = PNEUMATIC_DELAY;
	timer->Reset();
	timer->Start();
	PneumaticsDone = false;
	open_close = false;
}

// Called repeatedly when this Command is scheduled to run
void PneumaticsAuto::Execute()
{
	//printf("PneumaticsAuto execute\n");
	time_timer = timer->Get();
	if(time_timer >= time_run)
	{
		PneumaticsDone = true;
	}
	else
	{
		PneumaticsDone = false;
	}

	if(open_close == true)
	{
		if(leftArm == true)
		{
			Robot::gripperArm->leftGripper->Set(Robot::gripperArm->leftGripper->kForward);
		}
		if(rightArm == true)
		{
			Robot::gripperArm->rightGripper->Set(Robot::gripperArm->rightGripper->kForward);
		}
	}
	if(open_close == false)
	{
		if(leftArm == true)
		{
			Robot::gripperArm->leftGripper->Set(Robot::gripperArm->leftGripper->kReverse);
		}
		if(rightArm == true)
		{
			Robot::gripperArm->rightGripper->Set(Robot::gripperArm->rightGripper->kReverse);
		}
	}
	//printf("PnuematicsDone = %d", PneumaticsDone);
}

// Make this return true when this Command no longer needs to run execute()
bool PneumaticsAuto::IsFinished()
{
	//printf("PneumaticsAuto isFinished\n");
	return PneumaticsDone;
}

// Called once after isFinished returns true
void PneumaticsAuto::End()
{
	//printf("PneumaticsAuto end\n");
	timer->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void PneumaticsAuto::Interrupted()
{
	//printf("PneumaticsAuto interuppted\n");
}
