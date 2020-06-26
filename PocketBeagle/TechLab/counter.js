#!/usr/bin/env node
// Pre-steps (may need to eventually put in this program):
//   * Make sure /sys/class/gpio/gpio45 is exported
//   * Make sure /sys/class/gpio/gpio117 is exported
//   * Make sure pinmux is in gpio mode
//   * Make sure gpio is in input mode

var Epoll = require('epoll').Epoll;
var fs = require('fs');

var L_BUTTON_PATH = "/sys/class/gpio/gpio45"; // P2_33
var R_BUTTON_PATH = "/sys/class/gpio/gpio117"; // P1_29
var debounceActive = false;
var counter = 0;
var buffer = Buffer.alloc(1);

writeDigit(0, getDigit(0));
writeDigit(1, getDigit(0));

var L_BUTTON = fs.openSync(L_BUTTON_PATH + "/value", "r");
var R_BUTTON = fs.openSync(R_BUTTON_PATH + "/value", "r");

// Set interrupt mode
fs.writeFileSync(L_BUTTON_PATH + "/edge", "falling");
fs.writeFileSync(R_BUTTON_PATH + "/edge", "falling");

// Attach interrupts
var left_poller = new Epoll(leftPress);
left_poller.add(L_BUTTON, Epoll.EPOLLPRI);
var right_poller = new Epoll(rightPress);
right_poller.add(R_BUTTON, Epoll.EPOLLPRI);

console.log('Hit ^C to exit');

function leftPress(err, fd, events) {
    fs.readSync(fd, buffer, 0, 1, 0); // Clears interrupt

    if(debounceActive) {
        return;
    }
    debounceActive = true;
    //console.log("L pressed");

    counter--;
    if(counter < 0) counter = 0;
    writeCount(counter);

    setTimeout(clearDebounce, 150);
    return true;
}

function rightPress(err, fd, events) {
    fs.readSync(fd, buffer, 0, 1, 0); // Clears interrupt
    
    if(debounceActive) {
        return;
    }
    debounceActive = true;
    //console.log("R pressed");

    counter++;
    if(counter > 0xff) counter = 0xff;
    writeCount(counter);

    setTimeout(clearDebounce, 150);
    return true;
}

function clearDebounce() {
    debounceActive = false;
}

function writeCount(count) {
    writeDigit(0, getDigit((count>>4)&0xf));
    writeDigit(1, getDigit(count&0xf));
}

function getDigit(ch) {
    var segs;
    switch(ch) {
        case 0:
            segs = 0b0111111;
            break;
        case 1:
            segs = 0b0000110;
            break;
        case 2:
            segs = 0b1011011;
            break;
        case 3:
            segs = 0b1001111;
            break;
        case 4:
            segs = 0b1100110;
            break;
        case 5:
            segs = 0b1101101;
            break;
        case 6:
            segs = 0b1111101;
            break;
        case 7:
            segs = 0b0000111;
            break;
        case 8:
            segs = 0b1111111;
            break;
        case 9:
            segs = 0b1101111;
            break;
        case 0xa:
        case 'a':
            segs = 0b1110111;
            break;
        case 0xb:
        case 'b':
            segs = 0b1111100;
            break;
        case 0xc:
        case 'c':
            segs = 0b0111001;
            break;
        case 0xd:
        case 'd':
            segs = 0b1011110;
            break;
        case 0xe:
        case 'e':
            segs = 0b1111001;
            break;
        case 0xf:
        case 'f':
            segs = 0b1110001;
            break;
        case 'g':
            segs = 0b0000000;
            break;
        case 'h':
            segs = 0b1010101;
            break;
        case 'i':
            segs = 0b0101010;
            break;
        default:
            segs = 0;
            break;
    }

    return segs;
}

function writeDigit(digit, segs) {
    var index = digit * 8;
    var seg;
    for(var i = 0; i < 7; i++) {
        seg = (segs >> i) & 1;
        fs.writeFileSync("/sys/class/leds/techlab::seg" + (index+i) + "/brightness", seg)
    }
}