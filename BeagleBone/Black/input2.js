#!/usr/bin/env node
////////////////////////////////////////
//	input2.js
//      Responds to changes on P8_19 and P9_16 via callbacks.
//	Wiring:	Connect P8_19 to P9_14 with a 1kOhm resistor
//          Connect P9_15 to P9_16 with a 1kOhm resistor
//	Setup:	
//	See:	
////////////////////////////////////////
const fs = require('fs');
var Epoll = require('epoll').Epoll;

const outputPin  = '/sys/class/gpio/gpio40/value'; // P9_14
const outputPin2 = '/sys/class/gpio/gpio51/value'; // P9_16
const inputPin  = fs.openSync("/sys/class/gpio/gpio22/value", "r") // P8_19
const inputPin2 = fs.openSync("/sys/class/gpio/gpio48/value", "r") // P9_15
const ledPin  = '/sys/class/leds/beaglebone:green:usr3/brightness'; //usr3
const ledPin2 = '/sys/class/leds/beaglebone:green:usr2/brightness'; //usr2
const mydelay = 100;
const mydelay2 = 33;
var toggleState  = 0;   // b.LOW
var toggleState2 = 0;   // b.LOW
var buffer  = Buffer.alloc(1);
var buffer2 = Buffer.alloc(1);

console.log('Please connect ' + inputPin + ' to ' + outputPin +
    ' with a 1kOhm resistor');
console.log('Please connect ' + inputPin2 + ' to ' + outputPin2 +
    ' with a 1kOhm resistor');

// Set P8_19 and P9_15 as input pin
fs.writeFileSync('/sys/class/gpio/gpio22/edge', "both");
fs.writeFileSync('/sys/class/gpio/gpio48/edge', "both");

fs.writeFileSync(outputPin, 0);     // b.digitalWrite(outputPin, b.LOW);
fs.writeFileSync(outputPin2, 0);    // b.digitalWrite(outputPin2, b.LOW);

// Create a new Epoll. The callback is the interrupt handler.
var poller = new Epoll(inputHandler);
var poller2 = new Epoll(inputHandler2);
// Start watching for interrupts.
poller.add(inputPin, Epoll.EPOLLPRI);
poller2.add(inputPin2, Epoll.EPOLLPRI);

toggle();
toggle2();

function inputHandler(err, fd, events) {
    // Check for error
    if (err) {
        console.log('err = ' + err);
        return;
    }
    // Read GPIO value file. Reading also clears the interrupt.
    fs.readSync(fd, buffer, 0, 1, 0);
    fs.writeFileSync(ledPin, buffer);
}

function inputHandler2(err, fd, events) {
    // Check for error
    if (err) {
        console.log('err = ' + err);
        return;
    }
    // Read GPIO value file. Reading also clears the interrupt.
    fs.readSync(fd, buffer2, 0, 1, 0);
    fs.writeFileSync(ledPin2, buffer2);
}

function toggle() {
    toggleState = (toggleState == 0) ? 1 : 0;
    fs.writeFileSync(outputPin, toggleState);
    setTimeout(toggle, mydelay);    
}

function toggle2() {
    toggleState2 = (toggleState2 == 0) ? 1 : 0;
    fs.writeFileSync(outputPin2, toggleState2);
    setTimeout(toggle2, mydelay2);
}
