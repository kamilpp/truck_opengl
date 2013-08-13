#include "vehicle.h"

Vehicle::Vehicle() {
	following_vehicle = NULL;
    this->left_steering_wheel = NULL;
    this->right_steering_wheel = NULL;
}

void Vehicle::move(direct_t X) {
    this->body->move(X);
    for (unsigned i = 0; i < this->left_wheels.size(); ++i) {
        this->left_wheels[i]->move(X);
        this->right_wheels[i]->move(X);
        this->left_wheels[i]->rotate((direct_t)(-X));
        this->right_wheels[i]->rotate(X);
    }
    if(this->left_steering_wheel) {
        this->left_steering_wheel->move(X);
        this->left_steering_wheel->rotate((direct_t)(-X));
    }
    if(this->right_steering_wheel) {
        this->right_steering_wheel->move(X);
        this->right_steering_wheel->rotate(X);
    }
    if (this->following_vehicle) {
        this->following_vehicle->move(X);
    }
}

Vehicle::~Vehicle() {
	left_wheels.clear();
	right_wheels.clear();
	delete body;
}
