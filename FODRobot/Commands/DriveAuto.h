#ifndef DRIVEAUTO_H
#define DRIVEAUTO_H

#include "Commands/Subsystem.h"
#include "../Robot.h"

/**
 *
 *
 * @author Drew
 */
class DriveAuto: public Command {
public:
	DriveAuto(double, double, double, double, double);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	Timer* timer;
	double speed_side;
	double speed_fow;
	double speed_rot;
	double imu_yaw;
	double time_run;
	double time_timer;
	bool isDoneDriveAuto;
};

#endif
