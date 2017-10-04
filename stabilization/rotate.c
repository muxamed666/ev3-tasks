int leftMotor = 0;
int rightMotor = 0;

void rotate_left()
{
	leftMotor = -10;
	rightMotor = 100;

	motor[motorA] = leftMotor;
	motor[motorB] = rightMotor;

	wait1Msec(1000);
}


task main()
{
	rotate_left();
}
