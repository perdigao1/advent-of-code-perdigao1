# test progress
import random

def rec1(nrec, progress0, progressDiv):

    print(f"progress:{progress0}, nrec:{nrec}, progressDiv:{progressDiv}")
    if nrec> random.randint(3,8):
        return
    progressDiv1=progressDiv/5
    progress1=progress0
    for i in range(5):
        nrec1=nrec+1
        #progress0+=progressDiv1
        rec1(nrec1, progress1, progressDiv1)
        progress1+=progressDiv1
    return

rec1(0, 0.0, 100.0)