package Cell;


/**
* Cell/StationOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Cell.idl
* Wednesday, May 16, 2018 1:27:36 PM MSK
*/


/* ????????? ??????? ??????? */
public interface StationOperations 
{

  /* ??? ????????? ?????? ?????? ???????????? ?????? TubeCallback */
  int register (Cell.TubeCallback objRef, String phoneNum);

  /* ????????? ????????? message ?? ?????? fromNum ? ?????? toNum */
  int sendSMS (String fromNum, String toNum, String message);
} // interface StationOperations
