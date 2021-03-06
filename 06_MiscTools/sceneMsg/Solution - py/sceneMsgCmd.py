#
# Copyright (C) 
# 
# File: sceneMsgCmd.py
#
# Dependency Graph Node: 
#
# Author: Maya Plug-in Wizard 2.0


import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import sys

kPluginNodeTypeName = "sceneMsgCmd"


IDs = OpenMaya.MCallbackIdArray()


def removeCallback(ID):

	for i in range(ID.length()):
		try:
			OpenMaya.MMessage.removeCallback( ID[i] )
		except:
			sys.stderr.write( "Failed to remove callback\n" )
			raise


def sceneMsgCmd():
	#- Clean up callback id array
	IDs.clear()

#- Message callbacks
def openCallback(clienData):
	print "The callback registered for MSceneMessage::kBeforeOpen is executed."

def newCallback(clienData):
	print "The callback registered for MSceneMessage::kAfterNew is executed."


def saveCheckCallback(retCode, clienData):
	print "The callback registered for MSceneMessage::kBeforeSaveCheck is executed."

	#- Abort the operation by setting retCode to point to false
	retCode = True
	#    OpenMaya.MScriptUtil.setBool(retCode, False)

	print "Abort current operations.../n"	

class sceneMsgCmd(OpenMayaMPx.MPxCommand):

	def __init__(self):
		OpenMayaMPx.MPxCommand.__init__(self)
		
	def doIt(self,argList):
		return self.redoIt()
	
	def redoIt(self):
		#- Register callback for MSceneMessage::kBeforeOpen message
		openCallbackId = OpenMaya.MSceneMessage.addCallback(OpenMaya.MSceneMessage.kBeforeOpen,openCallback, self)
		IDs.append(openCallbackId)

		#- Register callback for MSceneMessage::kAfterNew message
		newCallbackId = OpenMaya.MSceneMessage.addCallback(OpenMaya.MSceneMessage.kAfterNew,newCallback, self)
		IDs.append(newCallbackId)

		#- Register callback for MSceneMessage::kBeforeSave message
		saveCheckCallbackId = OpenMaya.MSceneMessage.addCheckCallback(OpenMaya.MSceneMessage.kBeforeSaveCheck,saveCheckCallback, self)
		IDs.append(saveCheckCallbackId)
		
	def undoIt(self):
		if IDs.length()!= 0: 
			removeCallback(IDs)
			
# Creator
def nodeCreator():
	return OpenMayaMPx.asMPxPtr( sceneMsgCmd() )

# Initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.registerCommand( kPluginNodeTypeName, nodeCreator)
	except:
		sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
		raise

# Uninitialize the script plug-in
def uninitializePlugin(mobject):

	# Remove the callback
	if IDs.length()!= 0:
		removeCallback( IDs )
		
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterCommand( kPluginNodeTypeName )
	except:
		sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeTypeName )
		raise
		