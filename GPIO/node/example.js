import GPIO from './index.js';

const gpio = new GPIO();

let i = 0;
setInterval(blink, 500);
function blink(){
    const val = i % 1 ? '11' : '00';
    gpio.setAll(val);
}


//gpio.set(0, 1);