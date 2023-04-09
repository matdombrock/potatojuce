import fs from 'fs';

class GPIO{
    constructor(){
        this._fifoPathIn = '/tmp/pgpio-in';
        this._fifoPathOut = '/tmp/pgpio-out';
        this._writer = fs.createWriteStream(this._fifoPathIn);
    }
    add(chipName, lineNumber, writeMode='r'){
        this._writer.write(`${chipName} ${lineNumber} ${writeMode}\n`);
    }
    set(pinIndex, value){
        this._writer.write(`set ${pinIndex} ${value}\n`);
    }
    setAll(bitString){
        this._writer.write(bitString+'\n');
    }
    pwm(pinIndex, value){
        this._writer.write(`pwm ${pinIndex} ${value}\n`);
    }
    close(){
        this._writer.end();
    }
}

export default GPIO;

