#!/usr/bin/env node
var fs = require('fs');
var pin = "P1_19"
var AIN0 = '/sys/bus/iio/devices/iio:device0/in_voltage0_raw'; // Pin: P1_19

var maxValue = 4095;  // maximum value for analog read

console.log('Hit ^C to stop');
doAnalogRead();

function printStatus(err, x) {
  if(err) {console.log('Got error: ' + err); return;};

  var value = parseInt(x, 10) / maxValue;

  process.stdout.write(pin + ": " + (value*100).toFixed(1) +
    '%, ' + (1.8*value).toFixed(3) + 'V   \r');
  setTimeout(doAnalogRead, 100);
}

function doAnalogRead() {
  fs.readFile(AIN0, printStatus);
}