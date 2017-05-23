import sys
import os
import snap

#Load Tables
context = snap.TTableContext()

friends_file = "friends.txt"
schema = snap.Schema()
schema.Add(snap.TStrTAttrPr("Student1ID", snap.atInt))
schema.Add(snap.TStrTAttrPr("Student2ID", snap.atInt))
friends_table = snap.TTable.LoadSS(schema, friends_file, context, "\t", snap.TBool(False))

enrollments_file = "enrollments.txt"
schema = snap.Schema()
schema.Add(snap.TStrTAttrPr("StudentID", snap.atInt))
schema.Add(snap.TStrTAttrPr("CourseID", snap.atInt))
enrollments_table = snap.TTable.LoadSS(schema, enrollments_file, context, "\t", snap.TBool(False))

advisors_file = "advisors.txt"
schema = snap.Schema()
schema.Add(snap.TStrTAttrPr("StudentID", snap.atInt))
schema.Add(snap.TStrTAttrPr("ProfID", snap.atInt))
advisors_table = snap.TTable.LoadSS(schema, advisors_file, context, "\t", snap.TBool(False))

prereqs_file = "prereqs.txt"
schema = snap.Schema()
schema.Add(snap.TStrTAttrPr("CourseID", snap.atInt))
schema.Add(snap.TStrTAttrPr("PrereqCourseID", snap.atInt))
prereqs_table = snap.TTable.LoadSS(schema, prereqs_file, context, "\t", snap.TBool(False))

instructors_file = "instructors.txt"
schema = snap.Schema()
schema.Add(snap.TStrTAttrPr("CourseID", snap.atInt))
schema.Add(snap.TStrTAttrPr("ProfID", snap.atInt))
instructors_table = snap.TTable.LoadSS(schema, instructors_file, context, "\t", snap.TBool(False))

#Save Table
dstfile = "tables.bin"
FOut = snap.TFOut(dstfile)

friends_table.Save(FOut)
FOut.Flush()
enrollments_table.Save(FOut)
FOut.Flush()
advisors_table.Save(FOut)
FOut.Flush()
prereqs_table.Save(FOut)
FOut.Flush()
instructors_table.Save(FOut)
FOut.Flush()
context.Save(FOut)
FOut.Flush()

#Create TMMNet
mmnet = snap.TMMNet.New()
nodeattrv = snap.TStrV()
snap.LoadModeNetToNet(mmnet, "students", advisors_table, "StudentID", nodeattrv)
snap.LoadModeNetToNet(mmnet, "courses", instructors_table, "CourseID", nodeattrv)
snap.LoadModeNetToNet(mmnet, "profs", advisors_table, "ProfID", nodeattrv)

edgeattrv = snap.TStrV()
snap.LoadCrossNetToNet(mmnet, "students", "students", "friends", friends_table, "Student1ID", "Student2ID", edgeattrv)
snap.LoadCrossNetToNet(mmnet, "students", "profs", "advisors", advisors_table, "StudentID", "ProfID", edgeattrv)
snap.LoadCrossNetToNet(mmnet, "students", "courses", "enrollments", enrollments_table, "StudentID", "CourseID", edgeattrv)
snap.LoadCrossNetToNet(mmnet, "courses", "courses", "prereqs", prereqs_table, "CourseID", "PrereqCourseID", edgeattrv)
snap.LoadCrossNetToNet(mmnet, "courses", "profs", "instructors", instructors_table, "CourseID", "ProfID", edgeattrv)

# Convert to TNEANet

crossnetids = snap.TIntV()
crossnetids.Add(mmnet.GetCrossId("friends"))
crossnetids.Add(mmnet.GetCrossId("advisors"))
crossnetids.Add(mmnet.GetCrossId("enrollments"))
crossnetids.Add(mmnet.GetCrossId("prereqs"))
crossnetids.Add(mmnet.GetCrossId("instructors"))

# These are mappings consisting of triples of (modeid, old attribute name, new attribute name)
nodeattrmapping = snap.TIntStrStrTrV()
edgeattrmapping = snap.TIntStrStrTrV()

pneanet = mmnet.ToNetwork(crossnetids, nodeattrmapping, edgeattrmapping)

print pneanet.GetNodes(), pneanet.GetEdges()

print snap.GetClustCf(pneanet)

