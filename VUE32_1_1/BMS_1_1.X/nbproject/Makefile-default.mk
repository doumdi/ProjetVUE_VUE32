#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/vue32_timers.o ${OBJECTDIR}/_ext/1360937237/communication.o ${OBJECTDIR}/Board.o ${OBJECTDIR}/Spi.o ${OBJECTDIR}/BQ_Com.o ${OBJECTDIR}/BQ_Branch.o ${OBJECTDIR}/BQ.o ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o ${OBJECTDIR}/BMS.o ${OBJECTDIR}/BMS_Memory.o ${OBJECTDIR}/CANFunctions.o ${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/1360937237/interrupts.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d ${OBJECTDIR}/_ext/1360937237/communication.o.d ${OBJECTDIR}/Board.o.d ${OBJECTDIR}/Spi.o.d ${OBJECTDIR}/BQ_Com.o.d ${OBJECTDIR}/BQ_Branch.o.d ${OBJECTDIR}/BQ.o.d ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d ${OBJECTDIR}/BMS.o.d ${OBJECTDIR}/BMS_Memory.o.d ${OBJECTDIR}/CANFunctions.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/_ext/1360937237/interrupts.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/vue32_timers.o ${OBJECTDIR}/_ext/1360937237/communication.o ${OBJECTDIR}/Board.o ${OBJECTDIR}/Spi.o ${OBJECTDIR}/BQ_Com.o ${OBJECTDIR}/BQ_Branch.o ${OBJECTDIR}/BQ.o ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o ${OBJECTDIR}/BMS.o ${OBJECTDIR}/BMS_Memory.o ${OBJECTDIR}/CANFunctions.o ${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/1360937237/interrupts.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX575F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/vue32_timers.o: ../src/vue32_timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/vue32_timers.o ../src/vue32_timers.c   
	
${OBJECTDIR}/_ext/1360937237/communication.o: ../src/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/communication.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/communication.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/communication.o.d" -o ${OBJECTDIR}/_ext/1360937237/communication.o ../src/communication.c   
	
${OBJECTDIR}/Board.o: Board.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Board.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Board.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/Board.o.d" -o ${OBJECTDIR}/Board.o Board.c   
	
${OBJECTDIR}/Spi.o: Spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Spi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/Spi.o.d" -o ${OBJECTDIR}/Spi.o Spi.c   
	
${OBJECTDIR}/BQ_Com.o: BQ_Com.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ_Com.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ_Com.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ_Com.o.d" -o ${OBJECTDIR}/BQ_Com.o BQ_Com.c   
	
${OBJECTDIR}/BQ_Branch.o: BQ_Branch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ_Branch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ_Branch.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ_Branch.o.d" -o ${OBJECTDIR}/BQ_Branch.o BQ_Branch.c   
	
${OBJECTDIR}/BQ.o: BQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ.o.d" -o ${OBJECTDIR}/BQ.o BQ.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_Common.o: ../src/NETV32_Common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o ../src/NETV32_Common.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o: ../src/NETV32_Shared.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o ../src/NETV32_Shared.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o: ../src/NETV32_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o ../src/NETV32_CANDriver.c   
	
${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o: ../src/VUE32_Utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d" -o ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o ../src/VUE32_Utils.c   
	
${OBJECTDIR}/BMS.o: BMS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BMS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BMS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BMS.o.d" -o ${OBJECTDIR}/BMS.o BMS.c   
	
${OBJECTDIR}/BMS_Memory.o: BMS_Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BMS_Memory.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BMS_Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BMS_Memory.o.d" -o ${OBJECTDIR}/BMS_Memory.o BMS_Memory.c   
	
${OBJECTDIR}/CANFunctions.o: CANFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/CANFunctions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/CANFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/CANFunctions.o.d" -o ${OBJECTDIR}/CANFunctions.o CANFunctions.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
${OBJECTDIR}/_ext/1360937237/interrupts.o: ../src/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/interrupts.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" -o ${OBJECTDIR}/_ext/1360937237/interrupts.o ../src/interrupts.c   
	
else
${OBJECTDIR}/_ext/1360937237/vue32_timers.o: ../src/vue32_timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/vue32_timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/vue32_timers.o ../src/vue32_timers.c   
	
${OBJECTDIR}/_ext/1360937237/communication.o: ../src/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/communication.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/communication.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/communication.o.d" -o ${OBJECTDIR}/_ext/1360937237/communication.o ../src/communication.c   
	
${OBJECTDIR}/Board.o: Board.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Board.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Board.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/Board.o.d" -o ${OBJECTDIR}/Board.o Board.c   
	
${OBJECTDIR}/Spi.o: Spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Spi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/Spi.o.d" -o ${OBJECTDIR}/Spi.o Spi.c   
	
${OBJECTDIR}/BQ_Com.o: BQ_Com.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ_Com.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ_Com.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ_Com.o.d" -o ${OBJECTDIR}/BQ_Com.o BQ_Com.c   
	
${OBJECTDIR}/BQ_Branch.o: BQ_Branch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ_Branch.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ_Branch.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ_Branch.o.d" -o ${OBJECTDIR}/BQ_Branch.o BQ_Branch.c   
	
${OBJECTDIR}/BQ.o: BQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BQ.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BQ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BQ.o.d" -o ${OBJECTDIR}/BQ.o BQ.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_Common.o: ../src/NETV32_Common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_Common.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_Common.o ../src/NETV32_Common.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o: ../src/NETV32_Shared.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_Shared.o ../src/NETV32_Shared.c   
	
${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o: ../src/NETV32_CANDriver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o.d" -o ${OBJECTDIR}/_ext/1360937237/NETV32_CANDriver.o ../src/NETV32_CANDriver.c   
	
${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o: ../src/VUE32_Utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o.d" -o ${OBJECTDIR}/_ext/1360937237/VUE32_Utils.o ../src/VUE32_Utils.c   
	
${OBJECTDIR}/BMS.o: BMS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BMS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BMS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BMS.o.d" -o ${OBJECTDIR}/BMS.o BMS.c   
	
${OBJECTDIR}/BMS_Memory.o: BMS_Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/BMS_Memory.o.d 
	@${FIXDEPS} "${OBJECTDIR}/BMS_Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/BMS_Memory.o.d" -o ${OBJECTDIR}/BMS_Memory.o BMS_Memory.c   
	
${OBJECTDIR}/CANFunctions.o: CANFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/CANFunctions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/CANFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/CANFunctions.o.d" -o ${OBJECTDIR}/CANFunctions.o CANFunctions.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
${OBJECTDIR}/_ext/1360937237/interrupts.o: ../src/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/interrupts.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"C:/VUE/SVN/Code/Controle/Code_VUE32/VUE32_1_1/src" -MMD -MF "${OBJECTDIR}/_ext/1360937237/interrupts.o.d" -o ${OBJECTDIR}/_ext/1360937237/interrupts.o ../src/interrupts.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/BMS_1_1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
