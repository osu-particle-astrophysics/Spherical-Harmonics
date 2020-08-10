## this code is to collect the high scores from the files and take the average accross the run types then put that information into a master file 
import csv

with open("master.txt", "a") as master:
        for a in range(0,11):
                for b in range (1,11):
                        high = []
                        for x in range(1,11):
                                with open("Run_autorun"+str(a) + "."+ str(b)+ "." +str(x) + ".txt", "r") as txt:
                                        txt_read = csv.reader(txt, delimiter = " ")
                                        for i, row in enumerate(txt_read):
                                                if i == 5:	
                                                        high.append(float(row[3]))
                                                        
                        ave = 0.0
                        for y in range(0,10):
                                ave = ave + high[y]
                        ave = ave/10
                        master.write(str(a) + "." +str(b) + ".  " +str(ave) + "\n")
                        high.clear()
                        txt.close()
master.close()
