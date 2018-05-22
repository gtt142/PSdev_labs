import Cell.*;
import org.omg.CosNaming.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POA;
import java.io.*;

// ????? ?????? ?????????? ??????
class TubeCallbackServant extends TubeCallbackPOA {
 String myNum;	// ????? ??????

 // ??????????? ??????
 TubeCallbackServant (String num) {
   myNum = num;
   };

 // ????? ????????? ????????? ?????????
 public int sendSMS(String fromNum, String message) {
    System.out.println(myNum+": received message from "+fromNum+": \n"+message);
    return (0);
    };
 
 // ?????, ???????????? ????? ??????
 public String getNum() {
    return (myNum);
    };
  };

// ?????, ???????????? ??? ???????? ?????? ??????????
class ORBThread extends Thread {
  ORB myOrb;

  // ??????????? ??????
  ORBThread(ORB orb) {
    myOrb = orb;
    };

   // ????? ??????? ??????
   public void run() {
     myOrb.run();
     };
  };
 
// ?????, ??????????? ?????????? ??????
public class Tube {

  public static void main(String args[]) {
    try {
      BufferedReader inpt  = new BufferedReader(new InputStreamReader(System.in));
      System.out.println("Enter your number");
      String myNum = inpt.readLine();	// ????? ??????
      // ???????? ? ????????????? ORB
      ORB orb = ORB.init(args, null);

      //???????? ???????? ??? IDL-?????????? TubeCallback
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootPOA.the_POAManager().activate();
      TubeCallbackServant listener  = new TubeCallbackServant(myNum);
      rootPOA.activate_object(listener);
      // ????????? ?????? ?? ???????
      TubeCallback ref = TubeCallbackHelper.narrow(rootPOA.servant_to_reference(listener));
      
      // ????????? ????????? ??????????
      org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
      NamingContext ncRef = NamingContextHelper.narrow(objRef);
      
      // ?????????????? ????? ??????? ??????? ? ????????? ??????
      NameComponent nc = new NameComponent("BaseStation", "");
      NameComponent path[] = {nc};
      Station stationRef = StationHelper.narrow(ncRef.resolve(path));

      // ??????????? ?????? ? ??????? ???????
        int registerStatus = 0;
      registerStatus = stationRef.register(ref, myNum);
      if (registerStatus != 1) {
          System.out.println("This number already registered");
          return;
      }
      System.out.println("Tube is registered by base station");

      // ?????? ORB ? ????????? ?????? ??????????
      // ??? ????????????? ??????? ??????
      ORBThread orbThr = new ORBThread(orb);
      orbThr.start();

      // ??????????? ???? ?????? ????? ? ?????????? ? ??????? ??
      // ??????? ???????
      
      String msg;
      while (true) {
          StringBuilder text = new StringBuilder();        
          System.out.println("Write a message");
          try {
            msg = inpt.readLine();
            while (msg != null) {
              text.append(msg).append('\n');
              msg = inpt.readLine();
            }
        } catch (IOException e) {
          e.printStackTrace();
        }
        System.out.println("Write a receiver number");
        String num = inpt.readLine();
        stationRef.sendSMS(myNum, num, text.toString());
        // ???????? ????????: ????? ?????????? 7890 ? ????????? ?????
        // ?????????? ??????? ??????? ???? ?? ??????
        }

      } catch (Exception e) {
	 e.printStackTrace();
      };


    };

  }; 
