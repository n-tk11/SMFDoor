from usb_setup import find_mcu_boards, McuBoard, PeriBoard
from flask import Flask, render_template, redirect, url_for
import random,time
devices = find_mcu_boards()
mcu = McuBoard(devices[0])
peri = PeriBoard(mcu)
app = Flask(__name__)

NOTHING = 3
CORRECT = 4
WAIT = 5
WRONG = 6

passw = [0,0,0,0]
passc = 0
upass = [0,0,0,0]
stime = 0
rtime = 0
isWrong = False
isCorrect = False

def randpass():
    for i in range(0,4):
        passw[i]=random.randint(1,4)


@app.route("/")
def home():
    peri.set_led_servo(NOTHING)
    return render_template('home.html',lmode=1)

@app.route("/locked")
def home_locked():
    peri.set_led_servo(NOTHING)
    return render_template('home.html',lmode=2)

@app.route("/code")
def code():
    randpass()
    global passc,upass,stime,rtime
    rtime = 2
    stime = time.time()
    passc = 0
    upass = []
    peri.set_led_servo(WAIT)
    return redirect(url_for('judge'))


@app.route("/code/res")
def judge():
    global passc,upass,isWrong,passw,stime,rtime,isCorrect
    if(time.time()-stime >= 120):
        return redirect(url_for('home_locked'))
    elif(time.time()-stime >=60):
        rtime = 1
    if(isWrong == True and passc == 4):
        randpass()
        passc = 0
        upass = []
        isWrong = False
        peri.set_led_servo(WRONG)
    if(isCorrect == True and passc == 4):
        peri.set_led_servo(CORRECT)
        isCorrect = False
        return redirect(url_for('home'))
    if(passc < 4):
        userInput = peri.get_switch()
        if(userInput!=0):
            print(userInput)
            passc+=1
            upass.append(userInput)
        peri.set_led_servo(WAIT)
        return render_template('code.html',passw=passw,mode=1,rtime=rtime)
    elif (passc==4):
        isCorrect = True
        for i in range(0,4):
            if(upass[i] != passw[i]):
                isCorrect = False
                break
        if(isCorrect):
            return render_template('code.html',passw=passw,mode=2,rtime=rtime)
        else:
            isWrong = True
            return render_template('code.html',passw=passw,mode=3,rtime=rtime) 


app.run(debug=True, port=80, host="0.0.0.0")
