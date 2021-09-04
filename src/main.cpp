#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string>

using namespace std;

// DEFINE DEVICES
/////////////////////////////////////////////

pros::Controller master(CONTROLLER_MASTER);

pros::Motor motor1(5);

/////////////////////////////////////////////

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	pros::lcd::initialize();

	float msStop = 5000;
	float msNow;
	float t;
	float curve;
	int volts;

	while (pros::millis() < msStop)
	{
		msNow = pros::millis();
		t = msNow / msStop * 2 * M_PI;
		curve = sin(3 * t);
		volts = floor(127 * curve);

		if (volts < -127 || volts > 127)
		{
			pros::lcd::set_text(4, "voltage limit exceded");
			break;
		}

		pros::lcd::set_text(1, to_string(t));
		pros::lcd::set_text(2, to_string(curve));
		pros::lcd::set_text(3, to_string(volts));

		motor1.move(volts);

		pros::delay(20);
	}

	motor1.move(0);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol()
{
	pros::lcd::clear();

	while (true)
	{
		int joyLY = master.get_analog(ANALOG_LEFT_Y);
		bool A_btn = master.get_digital(DIGITAL_A);

		if (A_btn)
		{
			motor1.move(0);
		}
		else
		{
			motor1.move(joyLY);
		}

		pros::delay(20);
	}
}