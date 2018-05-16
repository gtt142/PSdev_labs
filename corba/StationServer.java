import Cell.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;
import java.util.HashMap;
import java.util.Map;

 

import java.util.Properties;

// ?????, ??????????? IDL-????????? ??????? ???????
class StationServant extends StationPOA {
  // ?????? ?????????????? ???? ???? ?????????? ?????
  // ?????? ???? ?????? ??? "????? - ????????? ??????"
  //TubeCallback tubeRef;
  //String tubeNum;

  Map<String, TubeCallback> tubeRefs = new HashMap<String, TubeCallback>();
  
  
  // ????? ??????????? ?????? ? ??????? ???????
  public int register (TubeCallback objRef, String phoneNum) {
     //tubeRef = objRef;
     //tubeNum = phoneNum;
     if(tubeRefs.containsKey(phoneNum)) {
        System.out.println("Number "+ phoneNum +" already registered");
        return (-1);
     }
     tubeRefs.put(phoneNum, objRef);
     System.out.println("Station: tube registered "+ phoneNum);
     return (1);
     };

  // ????? ????????? ????????? ?? ?????? ? ??????
  public int sendSMS (String fromNum, String toNum, String message) {
    System.out.println("Station: tube "+fromNum+" send a message "+toNum);
    // ????? ?????? ???? ????? ????????? ?????? ?? ?????? toNum
    TubeCallback tubeRef = tubeRefs.get(toNum);
    if (tubeRef == null) {
        System.out.println("Can't resolve a number: "+toNum);
        return (-1);
    }
    tubeRef.sendSMS(fromNum, message);
    return (1);
    };
  };

// ?????, ??????????? ?????? ??????? ???????
public class StationServer {

  public static void main(String args[]) {
    try{
      // ???????? ? ????????????? ORB
      ORB orb = ORB.init(args, null);

      // ????????? ?????? ? ????????????? POAManager
      POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootpoa.the_POAManager().activate();

      // ???????? ???????? ??? CORBA-??????? "??????? ???????" 
      StationServant servant = new StationServant();

      // ????????? ????????? ?????? ?? ???????
      org.omg.CORBA.Object ref = rootpoa.servant_to_reference(servant);
      Station sref = StationHelper.narrow(ref);
          
      org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
      NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

      // ?????????? ????????? ?????? ? ??????
      String name = "BaseStation";
      NameComponent path[] = ncRef.to_name( name );
      ncRef.rebind(path, sref);

      System.out.println("Server is ready and waiting ...");

      // ???????? ????????? ?? ???????? (??????)
      orb.run();
      } 
     catch (Exception e) {
        System.err.println("Server error: " + e);
        e.printStackTrace(System.out);
      };
    };
  }; 