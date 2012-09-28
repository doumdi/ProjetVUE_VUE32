/* 
 * File:   VUE32_Utils.h
 * Author: bedm2308
 *
 * Created on 27 septembre 2012, 10:20
 */

#ifndef VUE32_UTILS_H
#define	VUE32_UTILS_H

#define ON_MSG_TYPE(type) if (msg->msg_type == type){ unsigned char _temp;
#define ON_MSG_TYPE_RTR(type) if (msg->msg_remote == 1 && msg->msg_type == type){ unsigned char _temp;
#define ANSWER1(ressid, type1, var1)        \
if ( msg->msg_cmd == ressid) {              \
msg->msg_type = msg->msg_type + 1;          \
msg->msg_remote = 0;                        \
_temp = msg->msg_dest;                      \
msg->msg_dest = msg->msg_source;            \
if ( msg->msg_source == 0x3F )              \
    msg->msg_source = GetMyAddr();          \
else                                        \
    msg->msg_source = _temp;                \
msg->msg_data_length = sizeof(type1);       \
((type1*)msg->msg_data)[0] = var1;          \
netv_send_message(msg);                     \
}

#define ANSWER2(ressid, type1, var1, type2, var2)  \
if ( msg->msg_cmd == ressid) {              \
msg->msg_type = msg->msg_type + 1;          \
msg->msg_remote = 0;                        \
_temp = msg->msg_dest;                      \
msg->msg_dest = msg->msg_source;            \
if ( msg->msg_source == 0x3F )              \
    msg->msg_source = GetMyAddr();          \
else                                        \
    msg->msg_source = _temp;                \
msg->msg_data_length = sizeof(type1)+sizeof(type2); \
((type1*)msg->msg_data)[0] = var1;          \
((type2*)(msg->msg_data+sizeof(type1)))[0] = var2;  \
netv_send_message(msg);                     \
}

#define ANSWER3(ressid, type1, var1, type2, var2, type3, var3)  \
if ( msg->msg_cmd == ressid) {              \
msg->msg_type = msg->msg_type + 1;          \
msg->msg_remote = 0;                        \
_temp = msg->msg_dest;                      \
msg->msg_dest = msg->msg_source;            \
if ( msg->msg_source == 0x3F )              \
    msg->msg_source = GetMyAddr();          \
else                                        \
    msg->msg_source = _temp;                \
msg->msg_data_length = sizeof(type1)+sizeof(type2)+sizeof(type3);   \
((type1*)msg->msg_data)[0] = var1;          \
((type2*)(msg->msg_data+sizeof(type1)))[0] = var2;  \
((type3*)(msg->msg_data+sizeof(type1)+sizeof(type2)))[0] = var3;\
netv_send_message(msg);                     \
}

#define ACTION1(ressid, type1, var1) \
if ( msg->msg_cmd == ressid && msg->msg_data_length >= sizeof(type1)) {   \
var1 = ((type1*)msg->msg_data)[0];

#define ACTION2(ressid, type1, var1, type2, var2) \
if ( msg->msg_cmd == ressid && msg->msg_data_length >= sizeof(type1)+sizeof(type2)) {   \
var1 = ((type1*)msg->msg_data)[0];          \
var2 = ((type2*)(msg->msg_data+sizeof(type1)))[0];

#define ACTION3(ressid, type1, var1, type2, var2, type3, var3) \
if ( msg->msg_cmd == ressid && msg->msg_data_length >= sizeof(type1)+sizeof(type2)+sizeof(type3)) {   \
var1 = ((type1*)msg->msg_data)[0];          \
var2 = ((type2*)(msg->msg_data+sizeof(type1)))[0];  \
var3 = ((type3*)(msg->msg_data+sizeof(type1)+sizeof(type2)))[0];

#define END_OF_ACTION }

#define END_OF_MSG_TYPE }


#endif	/* VUE32_UTILS_H */
