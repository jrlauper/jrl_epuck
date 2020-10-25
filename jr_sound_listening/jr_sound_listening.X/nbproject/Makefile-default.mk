#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main_c.c ../e_init_port.c ../e_ad_conv.c ../e_micro.c ../e_led.c ../e_fast_copy.s ../e_fft.c ../e_input_signal.c ../e_twiddle_factors.c ../e_subtract_mean.s ../e_init_uart1.s ../e_uart1_rx_char.s ../e_uart1_tx_char.s ../btcom.c ../runfftlistener_mod.c ../e_I2C_protocol.c ../e_I2C_master_module.c ../e_lsm330.c ../utility_mod3.c ../e_prox.c ../e_freq_recognition.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main_c.o ${OBJECTDIR}/_ext/1472/e_init_port.o ${OBJECTDIR}/_ext/1472/e_ad_conv.o ${OBJECTDIR}/_ext/1472/e_micro.o ${OBJECTDIR}/_ext/1472/e_led.o ${OBJECTDIR}/_ext/1472/e_fast_copy.o ${OBJECTDIR}/_ext/1472/e_fft.o ${OBJECTDIR}/_ext/1472/e_input_signal.o ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o ${OBJECTDIR}/_ext/1472/e_subtract_mean.o ${OBJECTDIR}/_ext/1472/e_init_uart1.o ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o ${OBJECTDIR}/_ext/1472/btcom.o ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o ${OBJECTDIR}/_ext/1472/e_lsm330.o ${OBJECTDIR}/_ext/1472/utility_mod3.o ${OBJECTDIR}/_ext/1472/e_prox.o ${OBJECTDIR}/_ext/1472/e_freq_recognition.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main_c.o.d ${OBJECTDIR}/_ext/1472/e_init_port.o.d ${OBJECTDIR}/_ext/1472/e_ad_conv.o.d ${OBJECTDIR}/_ext/1472/e_micro.o.d ${OBJECTDIR}/_ext/1472/e_led.o.d ${OBJECTDIR}/_ext/1472/e_fast_copy.o.d ${OBJECTDIR}/_ext/1472/e_fft.o.d ${OBJECTDIR}/_ext/1472/e_input_signal.o.d ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d ${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d ${OBJECTDIR}/_ext/1472/e_init_uart1.o.d ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d ${OBJECTDIR}/_ext/1472/btcom.o.d ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d ${OBJECTDIR}/_ext/1472/e_lsm330.o.d ${OBJECTDIR}/_ext/1472/utility_mod3.o.d ${OBJECTDIR}/_ext/1472/e_prox.o.d ${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main_c.o ${OBJECTDIR}/_ext/1472/e_init_port.o ${OBJECTDIR}/_ext/1472/e_ad_conv.o ${OBJECTDIR}/_ext/1472/e_micro.o ${OBJECTDIR}/_ext/1472/e_led.o ${OBJECTDIR}/_ext/1472/e_fast_copy.o ${OBJECTDIR}/_ext/1472/e_fft.o ${OBJECTDIR}/_ext/1472/e_input_signal.o ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o ${OBJECTDIR}/_ext/1472/e_subtract_mean.o ${OBJECTDIR}/_ext/1472/e_init_uart1.o ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o ${OBJECTDIR}/_ext/1472/btcom.o ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o ${OBJECTDIR}/_ext/1472/e_lsm330.o ${OBJECTDIR}/_ext/1472/utility_mod3.o ${OBJECTDIR}/_ext/1472/e_prox.o ${OBJECTDIR}/_ext/1472/e_freq_recognition.o

# Source Files
SOURCEFILES=../main_c.c ../e_init_port.c ../e_ad_conv.c ../e_micro.c ../e_led.c ../e_fast_copy.s ../e_fft.c ../e_input_signal.c ../e_twiddle_factors.c ../e_subtract_mean.s ../e_init_uart1.s ../e_uart1_rx_char.s ../e_uart1_tx_char.s ../btcom.c ../runfftlistener_mod.c ../e_I2C_protocol.c ../e_I2C_master_module.c ../e_lsm330.c ../utility_mod3.c ../e_prox.c ../e_freq_recognition.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F6014A
MP_LINKER_FILE_OPTION=,--script="../p30f6014A.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main_c.o: ../main_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main_c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main_c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main_c.c  -o ${OBJECTDIR}/_ext/1472/main_c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main_c.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main_c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_init_port.o: ../e_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_init_port.c  -o ${OBJECTDIR}/_ext/1472/e_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_init_port.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_ad_conv.o: ../e_ad_conv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_ad_conv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_ad_conv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_ad_conv.c  -o ${OBJECTDIR}/_ext/1472/e_ad_conv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_ad_conv.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_ad_conv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_micro.o: ../e_micro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_micro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_micro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_micro.c  -o ${OBJECTDIR}/_ext/1472/e_micro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_micro.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_micro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_led.o: ../e_led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_led.c  -o ${OBJECTDIR}/_ext/1472/e_led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_led.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_fft.o: ../e_fft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fft.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_fft.c  -o ${OBJECTDIR}/_ext/1472/e_fft.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_fft.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_fft.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_input_signal.o: ../e_input_signal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_input_signal.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_input_signal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_input_signal.c  -o ${OBJECTDIR}/_ext/1472/e_input_signal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_input_signal.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_input_signal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_twiddle_factors.o: ../e_twiddle_factors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_twiddle_factors.c  -o ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/btcom.o: ../btcom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/btcom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/btcom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../btcom.c  -o ${OBJECTDIR}/_ext/1472/btcom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/btcom.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/btcom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/runfftlistener_mod.o: ../runfftlistener_mod.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../runfftlistener_mod.c  -o ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_I2C_protocol.o: ../e_I2C_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_I2C_protocol.c  -o ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_I2C_master_module.o: ../e_I2C_master_module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_I2C_master_module.c  -o ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_lsm330.o: ../e_lsm330.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_lsm330.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_lsm330.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_lsm330.c  -o ${OBJECTDIR}/_ext/1472/e_lsm330.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_lsm330.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_lsm330.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/utility_mod3.o: ../utility_mod3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/utility_mod3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/utility_mod3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../utility_mod3.c  -o ${OBJECTDIR}/_ext/1472/utility_mod3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/utility_mod3.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/utility_mod3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_prox.o: ../e_prox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_prox.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_prox.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_prox.c  -o ${OBJECTDIR}/_ext/1472/e_prox.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_prox.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_prox.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_freq_recognition.o: ../e_freq_recognition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_freq_recognition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_freq_recognition.c  -o ${OBJECTDIR}/_ext/1472/e_freq_recognition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d"      -g -D__DEBUG     -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/main_c.o: ../main_c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main_c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main_c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main_c.c  -o ${OBJECTDIR}/_ext/1472/main_c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main_c.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main_c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_init_port.o: ../e_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_init_port.c  -o ${OBJECTDIR}/_ext/1472/e_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_init_port.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_ad_conv.o: ../e_ad_conv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_ad_conv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_ad_conv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_ad_conv.c  -o ${OBJECTDIR}/_ext/1472/e_ad_conv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_ad_conv.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_ad_conv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_micro.o: ../e_micro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_micro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_micro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_micro.c  -o ${OBJECTDIR}/_ext/1472/e_micro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_micro.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_micro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_led.o: ../e_led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_led.c  -o ${OBJECTDIR}/_ext/1472/e_led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_led.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_fft.o: ../e_fft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fft.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_fft.c  -o ${OBJECTDIR}/_ext/1472/e_fft.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_fft.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_fft.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_input_signal.o: ../e_input_signal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_input_signal.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_input_signal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_input_signal.c  -o ${OBJECTDIR}/_ext/1472/e_input_signal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_input_signal.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_input_signal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_twiddle_factors.o: ../e_twiddle_factors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_twiddle_factors.c  -o ${OBJECTDIR}/_ext/1472/e_twiddle_factors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_twiddle_factors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/btcom.o: ../btcom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/btcom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/btcom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../btcom.c  -o ${OBJECTDIR}/_ext/1472/btcom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/btcom.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/btcom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/runfftlistener_mod.o: ../runfftlistener_mod.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../runfftlistener_mod.c  -o ${OBJECTDIR}/_ext/1472/runfftlistener_mod.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/runfftlistener_mod.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_I2C_protocol.o: ../e_I2C_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_I2C_protocol.c  -o ${OBJECTDIR}/_ext/1472/e_I2C_protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_I2C_protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_I2C_master_module.o: ../e_I2C_master_module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_I2C_master_module.c  -o ${OBJECTDIR}/_ext/1472/e_I2C_master_module.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_I2C_master_module.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_lsm330.o: ../e_lsm330.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_lsm330.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_lsm330.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_lsm330.c  -o ${OBJECTDIR}/_ext/1472/e_lsm330.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_lsm330.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_lsm330.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/utility_mod3.o: ../utility_mod3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/utility_mod3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/utility_mod3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../utility_mod3.c  -o ${OBJECTDIR}/_ext/1472/utility_mod3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/utility_mod3.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/utility_mod3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_prox.o: ../e_prox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_prox.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_prox.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_prox.c  -o ${OBJECTDIR}/_ext/1472/e_prox.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_prox.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_prox.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/e_freq_recognition.o: ../e_freq_recognition.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_freq_recognition.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../e_freq_recognition.c  -o ${OBJECTDIR}/_ext/1472/e_freq_recognition.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_freq_recognition.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/e_fast_copy.o: ../e_fast_copy.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fast_copy.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fast_copy.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_fast_copy.s  -o ${OBJECTDIR}/_ext/1472/e_fast_copy.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_fast_copy.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_fast_copy.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_subtract_mean.o: ../e_subtract_mean.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_subtract_mean.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_subtract_mean.s  -o ${OBJECTDIR}/_ext/1472/e_subtract_mean.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_init_uart1.o: ../e_init_uart1.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_uart1.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_init_uart1.s  -o ${OBJECTDIR}/_ext/1472/e_init_uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_init_uart1.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_init_uart1.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o: ../e_uart1_rx_char.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_uart1_rx_char.s  -o ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o: ../e_uart1_tx_char.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_uart1_tx_char.s  -o ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/e_fast_copy.o: ../e_fast_copy.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fast_copy.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_fast_copy.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_fast_copy.s  -o ${OBJECTDIR}/_ext/1472/e_fast_copy.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_fast_copy.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_fast_copy.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_subtract_mean.o: ../e_subtract_mean.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_subtract_mean.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_subtract_mean.s  -o ${OBJECTDIR}/_ext/1472/e_subtract_mean.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_subtract_mean.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_init_uart1.o: ../e_init_uart1.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_init_uart1.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_init_uart1.s  -o ${OBJECTDIR}/_ext/1472/e_init_uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_init_uart1.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_init_uart1.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o: ../e_uart1_rx_char.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_uart1_rx_char.s  -o ${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_uart1_rx_char.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o: ../e_uart1_tx_char.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../e_uart1_tx_char.s  -o ${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../../../library/codec" -I"../../../library/uart" -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/e_uart1_tx_char.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../libdsp-coff.a ../libdsp-elf.a  ../p30f6014A.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../libdsp-coff.a ../libdsp-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path="../C:/Program Files/Microchip/MPLAB C30/lib",--library-path=".",--no-force-link,--smart-io,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../libdsp-coff.a ../libdsp-elf.a ../p30f6014A.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../libdsp-coff.a ../libdsp-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=coff -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path="../C:/Program Files/Microchip/MPLAB C30/lib",--library-path=".",--no-force-link,--smart-io,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/jr_sound_listening.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=coff   -mdfp="${DFP_DIR}/xc16" 
	
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
