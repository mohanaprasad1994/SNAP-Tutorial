import snap
from random import *
import numpy as np
NUM_STUDENTS = 200
NUM_COURSES = 50
NUM_PROFS = 20

COURSE_OFFSET = NUM_STUDENTS
PROF_OFFSET = NUM_STUDENTS + NUM_COURSES
#students nodes numbered 0 to NUM_STUDENTS - 1
# friends = snap.GenRndPowerLaw(NUM_STUDENTS, 1.5)
num_friends = np.random.normal(10,2, NUM_STUDENTS)
fil = open("friends.txt",'w')
for i in range(NUM_STUDENTS):
	for j in range(int(num_friends[i])):
		fil.write(str(i)+"\t"+ str(randint(0, NUM_STUDENTS))+"\n")


fil = open("enrollments.txt",'w')
for i in range(NUM_STUDENTS):
	for j in range(15):
		course_id = randint(0, NUM_COURSES)
		fil.write(str(i)+"\t"+ str(course_id+ COURSE_OFFSET)+"\n")

fil = open("advisors.txt",'w')
for i in range(NUM_STUDENTS):
	num = randint(1,3) #1,2 or 3 advisors per student
	for j in range(num):
		prof_id = randint(0, NUM_PROFS)
		fil.write(str(i)+"\t"+ str(prof_id+ PROF_OFFSET)+"\n")

fil = open("prereqs.txt",'w')
for i in range(NUM_COURSES):
	num = randint(1,3) #1,2 or 3 advisors per student
	for j in range(num):
		course_id = randint(0, NUM_COURSES)
		fil.write(str(COURSE_OFFSET + i)+"\t"+ str(course_id+ COURSE_OFFSET)+"\n")

fil = open("instructors.txt",'w')
for i in range(NUM_COURSES):
	num = randint(1,2) #1,2 instructor per course
	for j in range(num):
		prof_id = randint(0, NUM_PROFS)
		fil.write(str(COURSE_OFFSET + i)+"\t"+ str(prof_id+ PROF_OFFSET)+"\n")


