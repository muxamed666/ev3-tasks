void rotate_left()
{
	int lMotor = -20;
	int rMotor = 80;

	motor[motorA] = lMotor;
	motor[motorB] = rMotor;

	wait1Msec(500);
}


void rotate_right()
{
	int lMotor = 80;
	int rMotor = -20;

	motor[motorA] = lMotor;
	motor[motorB] = rMotor;

	wait1Msec(500);
}
