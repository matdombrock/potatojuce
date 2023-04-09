import GPIO from './index.js';

const gpio = new GPIO();

let i = 0;
setInterval(blink, 500);
function blink(){
    // Blink on when `i` is odd
    const bitString = i % 2 ? '11' : '00';
    i++;
    gpio.setAll(bitString);
}