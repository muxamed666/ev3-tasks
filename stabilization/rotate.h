void rotate_left()
{
	int lMotor = -10;
	int rMotor = 100;

	motor[motorA] = lMotor;
	motor[motorB] = rMotor;

	wait1Msec(700);
}


void rotate_right()
{
	int lMotor = 100;
	int rMotor = -10;

	motor[motorA] = lMotor;
	motor[motorB] = rMotor;

	wait1Msec(700);
}
