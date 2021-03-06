package Cell;


/**
* Cell/_StationStub.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Cell.idl
* Wednesday, May 16, 2018 1:27:36 PM MSK
*/


/* ????????? ??????? ??????? */
public class _StationStub extends org.omg.CORBA.portable.ObjectImpl implements Cell.Station
{


  /* ??? ????????? ?????? ?????? ???????????? ?????? TubeCallback */
  public int register (Cell.TubeCallback objRef, String phoneNum)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("register", true);
                Cell.TubeCallbackHelper.write ($out, objRef);
                $out.write_string (phoneNum);
                $in = _invoke ($out);
                int $result = $in.read_long ();
                return $result;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                return register (objRef, phoneNum        );
            } finally {
                _releaseReply ($in);
            }
  } // register


  /* ????????? ????????? message ?? ?????? fromNum ? ?????? toNum */
  public int sendSMS (String fromNum, String toNum, String message)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("sendSMS", true);
                $out.write_string (fromNum);
                $out.write_string (toNum);
                $out.write_string (message);
                $in = _invoke ($out);
                int $result = $in.read_long ();
                return $result;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                return sendSMS (fromNum, toNum, message        );
            } finally {
                _releaseReply ($in);
            }
  } // sendSMS

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:Cell/Station:1.0"};

  public String[] _ids ()
  {
    return (String[])__ids.clone ();
  }

  private void readObject (java.io.ObjectInputStream s) throws java.io.IOException
  {
     String str = s.readUTF ();
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     org.omg.CORBA.Object obj = orb.string_to_object (str);
     org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl) obj)._get_delegate ();
     _set_delegate (delegate);
   } finally {
     orb.destroy() ;
   }
  }

  private void writeObject (java.io.ObjectOutputStream s) throws java.io.IOException
  {
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     String str = orb.object_to_string (this);
     s.writeUTF (str);
   } finally {
     orb.destroy() ;
   }
  }
} // class _StationStub
