#include <iostream>
#include "Snap.h"
#include "stdafx.h"

int main(int argc,char* argv[]) {

  PTable P_enrollments, P_friends, P_advisors, P_prereqs, P_instructors;
  //Load & save enrollments table
  {
    std::cout<<"Loading enrollments table"<<"\n";
    char src_file[] = "enrollments.txt";
    TTableContext Context;
    // Create schema.
    Schema PAA;
    PAA.Add(TPair<TStr,TAttrType>("StudentID", atInt));
    PAA.Add(TPair<TStr,TAttrType>("CourseID", atInt));

    TIntV RelevantCols;
    RelevantCols.Add(0); 
    RelevantCols.Add(1); 
    //Load table
    P_enrollments = TTable::LoadSS(PAA, src_file, &Context, RelevantCols);
    std::cout<<"enrollments table loaded"<<"\n";
    
    //Save enrollments table
    std::cout<<"Saving enrollments table"<<"\n";
    {
    TFOut SOut("enrollments.bin");
    P_enrollments->Save(SOut);
    Context.Save(SOut);
    }
    std::cout<<"enrollment table saved"<<"\n";
  }

  //Load & save friends table
 {
    std::cout<<"Loading friends table"<<"\n";
    char src_file[] = "friends.txt";
    TTableContext Context;
    // Create schema.
    Schema PAA;
    PAA.Add(TPair<TStr,TAttrType>("Student1ID", atInt));
    PAA.Add(TPair<TStr,TAttrType>("Student2ID", atInt));

    TIntV RelevantCols;
    RelevantCols.Add(0); 
    RelevantCols.Add(1); 
    //Load table
    P_friends = TTable::LoadSS(PAA, src_file, &Context, RelevantCols);
    std::cout<<"friends table loaded"<<"\n";
    
    //Save friends table
    std::cout<<"Saving friends table"<<"\n";
    {
    TFOut SOut("friends.bin");
    P_friends->Save(SOut);
    Context.Save(SOut);
    }
  }

  //Load & save advisors table
 {
    std::cout<<"Loading advisors table"<<"\n";
    char src_file[] = "advisors.txt";
    TTableContext Context;
    // Create schema.
    Schema PAA;
    PAA.Add(TPair<TStr,TAttrType>("StudentID", atInt));
    PAA.Add(TPair<TStr,TAttrType>("ProfID", atInt));

    TIntV RelevantCols;
    RelevantCols.Add(0); 
    RelevantCols.Add(1); 
    //Load table
    P_advisors = TTable::LoadSS(PAA, src_file, &Context, RelevantCols);
    std::cout<<"advisors table loaded"<<"\n";
    
    //Save advisors table
    std::cout<<"Saving advisors table"<<"\n";
    {
    TFOut SOut("advisors.bin");
    P_advisors->Save(SOut);
    Context.Save(SOut);
    }
  }

  //Load & save prereqs table
 {
    std::cout<<"Loading prereqs table"<<"\n";
    char src_file[] = "prereqs.txt";
    TTableContext Context;
    // Create schema.
    Schema PAA;
    PAA.Add(TPair<TStr,TAttrType>("CourseID", atInt));
    PAA.Add(TPair<TStr,TAttrType>("PrereqCourseID", atInt));

    TIntV RelevantCols;
    RelevantCols.Add(0); 
    RelevantCols.Add(1); 
    //Load table
    P_prereqs = TTable::LoadSS(PAA, src_file, &Context, RelevantCols);
    std::cout<<"prereqs table loaded"<<"\n";
    
    //Save prereqs table
    std::cout<<"Saving prereqs table"<<"\n";
    {
    TFOut SOut("prereqs.bin");
    P_prereqs->Save(SOut);
    Context.Save(SOut);
    }
  }

  //Load & save instructors table
 {
    std::cout<<"Loading instructors table"<<"\n";
    char src_file[] = "instructors.txt";
    TTableContext Context;
    // Create schema.
    Schema PAA;
    PAA.Add(TPair<TStr,TAttrType>("CourseID", atInt));
    PAA.Add(TPair<TStr,TAttrType>("ProfID", atInt));

    TIntV RelevantCols;
    RelevantCols.Add(0); 
    RelevantCols.Add(1); 
    //Load table
    P_instructors = TTable::LoadSS(PAA, src_file, &Context, RelevantCols);
    std::cout<<"instructors table loaded"<<"\n";
    
    //Save instructors table
    std::cout<<"Saving instructors table"<<"\n";
    {
    TFOut SOut("instructors.bin");
    P_instructors->Save(SOut);
    Context.Save(SOut);
    }
  }


  //Add modes
  TStrV NodeAttrV;
  PMMNet Graph = TMMNet::New();
  TSnap::LoadModeNetToNet(Graph, "Student", P_advisors, "StudentID", NodeAttrV);
  TSnap::LoadModeNetToNet(Graph, "Prof", P_instructors, "ProfID", NodeAttrV);
  TSnap::LoadModeNetToNet(Graph, "Course", P_prereqs, "CourseID", NodeAttrV);
  std::cout<<"Done loading all Modes\n";

  //Add crossnets
  TSnap::LoadCrossNetToNet(Graph, "Student", "Student", "Friends", P_friends, "Student1ID", "Student2ID", NodeAttrV);
  TSnap::LoadCrossNetToNet(Graph, "Student", "Course", "Enrollments", P_enrollments, "StudentID", "CourseID", NodeAttrV);
  TSnap::LoadCrossNetToNet(Graph, "Student", "Prof", "Advisors", P_advisors, "StudentID", "ProfID", NodeAttrV);
  TSnap::LoadCrossNetToNet(Graph, "Course", "Course", "Prereqs", P_prereqs, "CourseID", "PrereqCourseID", NodeAttrV);
  TSnap::LoadCrossNetToNet(Graph, "Course", "Prof", "Instructors", P_instructors, "CourseID", "ProfID", NodeAttrV);
  std::cout<<"Done loading all crossnets\n";
  std::cout<<"Done convertion to MMNet"<<"\n";


  TMMNet::TCrossNetI CI = Graph->GetCrossNetI(Graph->GetCrossId("Friends"));
  std::cout<< CI.GetCrossNet().GetEdges()<<std::endl;
  CI = Graph->GetCrossNetI(Graph->GetCrossId("Prereqs"));
  std::cout<<CI.GetCrossNet().GetEdges()<<std::endl;
  CI = Graph->GetCrossNetI(Graph->GetCrossId("Instructors"));
  std::cout<<CI.GetCrossNet().GetEdges()<<std::endl;

  //Convert to TNEANet
  std::cout<<"Converting to TNEANet\n";
  TIntV crossnetids;
  crossnetids.Add(Graph->GetCrossId("Friends"));
  crossnetids.Add(Graph->GetCrossId("Enrollments"));
  crossnetids.Add(Graph->GetCrossId("Advisors"));
  crossnetids.Add(Graph->GetCrossId("Prereqs"));
  crossnetids.Add(Graph->GetCrossId("Instructors"));

  TIntStrStrTrV nodeattrmapping;
  TIntStrStrTrV edgeattrmapping;
  PNEANet pneanet = Graph->ToNetwork(crossnetids, nodeattrmapping, edgeattrmapping);
  
  // TStrV crossnetnames;
  // crossnetnames.Add("Friends");
  // crossnetnames.Add("Enrollments");
  // crossnetnames.Add("Advisors");
  // crossnetnames.Add("Prereqs");
  // crossnetnames.Add("Instructors");

  // PNEANetMP pneanet = Graph->ToNetworkMP(crossnetnames);
  std::cout<<"Done converting to TNEANet"<<"\n";

  std::cout<<"Number of nodes: "<< pneanet->GetNodes()<<", Number of edges: "<<pneanet->GetEdges()<<std::endl;

  //Analytics on the TNEANet Graph

  //Get number of triangles
  std::cout<<"Number of Triangles: "<<TSnap::GetTriangleCnt(pneanet);

  return 0;

}
