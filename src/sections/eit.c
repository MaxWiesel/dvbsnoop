/*
$Id :

   -- EIT section
   -- Event Information Table
   -- ETSI EN 300 469  5.2.4

   (c) rasc


$Log: eit.c,v $
Revision 1.4  2003/10/16 19:02:29  rasc
some updates to dvbsnoop...
- small bugfixes
- tables updates from ETR 162

Revision 1.3  2002/08/17 20:36:12  obi
no more compiler warnings

Revision 1.2  2001/10/06 18:19:18  Toerli
Steuerzeichen entfernt. rasc wuerdest du mal bitte nen gescheiten unix-konformen Editor verwenden... windows editoren sind ungeeignet

Revision 1.1  2001/09/30 13:05:20  rasc
dvbsnoop v0.7  -- Commit to CVS



*/




#include "dvbsnoop.h"
#include "eit.h"
#include "descriptor.h"




void decode_EIT (u_char *b, int len)
{

 typedef struct  _EIT {
    u_int      table_id;
    u_int      section_syntax_indicator;		
    u_int      reserved_1;
    u_int      reserved_2;
    u_int      section_length;
    u_int      service_id;
    u_int      reserved_3;
    u_int      version_number;
    u_int      current_next_indicator;
    u_int      section_number;
    u_int      last_section_number;
    u_int      transport_stream_id;
    u_int      original_network_id;
    u_int      segment_last_section_number;
    u_int      last_table_id;

    // N1 EIT_LIST2

    unsigned long crc;
 } EIT;


 typedef struct _EIT_LIST2 {
    u_int      event_id;
    u_long     start_time_MJD;  // 16
    u_long     start_time_UTC;  // 24
    u_long     duration;
    u_int      running_status;
    u_int      free_CA_mode;
    u_int      descriptors_loop_length;

    // N2 descriptor

 } EIT_LIST2;



 EIT        e;
 EIT_LIST2  e2;
 int        len1,len2;


 
 e.table_id 			 = b[0];
 e.section_syntax_indicator	 = getBits (b, 0,  8,  1);
 e.reserved_1 			 = getBits (b, 0,  9,  1);
 e.reserved_2 			 = getBits (b, 0, 10,  2);
 e.section_length		 = getBits (b, 0, 12, 12);
 e.service_id			 = getBits (b, 0, 24, 16);
 e.reserved_3 			 = getBits (b, 0, 40,  2);
 e.version_number 		 = getBits (b, 0, 42,  5);
 e.current_next_indicator	 = getBits (b, 0, 47,  1);
 e.section_number 		 = getBits (b, 0, 48,  8);
 e.last_section_number 		 = getBits (b, 0, 56,  8);
 e.transport_stream_id		 = getBits (b, 0, 64, 16);
 e.original_network_id		 = getBits (b, 0, 80, 16);
 e.segment_last_section_number	 = getBits (b, 0, 96,  8);
 e.last_table_id		 = getBits (b, 0,104,  8);


 out_nl (3,"EIT-decoding....");
 out_S2B_NL (3,"Table_ID: ",e.table_id, dvbstrTableID (e.table_id));
 if (   e.table_id != 0x4E && e.table_id != 0x4F
     && !(e.table_id >= 0x50 && e.table_id <= 0x6F) ) {
   out_nl (3,"wrong Table ID");
   return;
 }

 out_SB_NL (3,"section_syntax_indicator: ",e.section_syntax_indicator);
 out_SB_NL (6,"reserved_1: ",e.reserved_1);
 out_SB_NL (6,"reserved_2: ",e.reserved_2);
 out_SW_NL (5,"Section_length: ",e.section_length);

 out_S2W_NL (3,"Service_ID: ",e.service_id,
	" --> refers to PMS program_number"); 

 out_SB_NL (6,"reserved_3: ",e.reserved_3);
 out_SB_NL (3,"Version_number: ",e.version_number);
 out_SB_NL (3,"Current_next_indicator: ",e.current_next_indicator);
 out_SB_NL (3,"Section_number: ",e.section_number);
 out_SB_NL (3,"Last_Section_number: ",e.last_section_number);


 out_SW_NL  (3,"Transport_stream_ID: ",e.transport_stream_id);
 out_S2W_NL (3,"Original_network_ID: ",e.original_network_id,
	 dvbstrOriginalNetwork_ID(e.original_network_id)); 
 out_SB_NL  (3,"Segment_last_Section_number: ",e.segment_last_section_number);
 out_S2B_NL (3,"Last_table_id: ",e.last_table_id,dvbstrTableID(e.last_table_id));


 // - header data after length value
 len1 = e.section_length - 11;
 b   += 14;

 indent (+1);
 while (len1 > 4) {

   e2.event_id			 = getBits (b, 0,  0, 16);
   e2.start_time_MJD		 = getBits (b, 0, 16, 16);
   e2.start_time_UTC		 = getBits (b, 0, 32, 24);
   e2.duration			 = getBits (b, 0, 56, 24);
   e2.running_status		 = getBits (b, 0, 80, 3);
   e2.free_CA_mode		 = getBits (b, 0, 83, 1);
   e2.descriptors_loop_length	 = getBits (b, 0, 84, 12);

   out_NL (3);
   out_SW_NL (3,"Event_ID: ",e2.event_id);

   out (3,"Start_time: "); 
	print_time40 (3,e2.start_time_MJD,e2.start_time_UTC);
	out_NL(3);
   out (3,"Duration: "); 
	print_time40 (3,0L,e2.duration);
	out_NL (3);
   out_S2B_NL (3,"Running_status: ",e2.running_status,
	dvbstrRunningStatus_FLAG (e2.running_status));
   out_S2B_NL (3,"Free_CA_mode: ",e2.free_CA_mode,
	(e2.free_CA_mode) ?"streams [partially] CA controlled" :"unscrambled");
   out_SB_NL (5,"Descriptors_loop_length: ",e2.descriptors_loop_length);


   b    += 12;
   len1 -= (12 + e2.descriptors_loop_length);
   len2 = e2.descriptors_loop_length;

   while (len2 > 0) {
      int x;

      x = descriptor (b);
      b    += x;
      len2 -= x;
   }

 } // while len1
 indent (-1);


 e.crc		 		 = getBits (b, 0, 0, 32);
 out_SL_NL (5,"CRC: ",e.crc);

}




