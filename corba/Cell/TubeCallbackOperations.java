package Cell;


/**
* Cell/TubeCallbackOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Cell.idl
* Wednesday, May 16, 2018 1:27:36 PM MSK
*/


/* ????????? ????????? ?????? ?????? */
public interface TubeCallbackOperations 
{

  /* ??????? ????????? message ?? ?????? fromNum */
  int sendSMS (String fromNum, String message);

  /* ??????? ???? ????? */
  String getNum ();
} // interface TubeCallbackOperations
