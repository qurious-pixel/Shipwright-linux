#include "OTRGlobals.h"
#include "ResourceMgr.h"
#include "Scene.h"
#include "message_data_static.h"
#include "Utils/StringHelper.h"
#include "global.h"
#include "vt.h"
#include <Text.h>

extern "C" MessageTableEntry* sNesMessageEntryTablePtr;
extern "C" MessageTableEntry* sGerMessageEntryTablePtr;
extern "C" MessageTableEntry* sFraMessageEntryTablePtr;
extern "C" MessageTableEntry* sStaffMessageEntryTablePtr;
//extern "C" MessageTableEntry* _message_0xFFFC_nes;

extern "C" void OTRMessage_Init()
{
	auto file_eng = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource("text/nes_message_data_static/nes_message_data_static"));
	auto file_ger = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource("text/ger_message_data_static/ger_message_data_static"));
	auto file_fra = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource("text/fra_message_data_static/fra_message_data_static"));

	sNesMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file_eng->messages.size());
	sGerMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file_ger->messages.size());
	sFraMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file_fra->messages.size());

	for (int i = 0; i < file_eng->messages.size(); i++)
	{
		sNesMessageEntryTablePtr[i].textId = file_eng->messages[i].id;
		sNesMessageEntryTablePtr[i].typePos = (file_eng->messages[i].textboxType << 4) | file_eng->messages[i].textboxYPos;
		sNesMessageEntryTablePtr[i].segment = file_eng->messages[i].msg.c_str();
		sNesMessageEntryTablePtr[i].msgSize = file_eng->messages[i].msg.size();

		if (file_eng->messages[i].id == 0xFFFC) {
			_message_0xFFFC_nes = (char*)file_eng->messages[i].msg.c_str();
		}
	}

	for (int i = 0; i < file_ger->messages.size(); i++)
	{
		sGerMessageEntryTablePtr[i].textId = file_ger->messages[i].id;
		sGerMessageEntryTablePtr[i].typePos = (file_ger->messages[i].textboxType << 4) | file_ger->messages[i].textboxYPos;
		sGerMessageEntryTablePtr[i].segment = file_ger->messages[i].msg.c_str();
		sGerMessageEntryTablePtr[i].msgSize = file_ger->messages[i].msg.size();

		if (file_ger->messages[i].id == 0xFFFC) {
			_message_0xFFFC_ger = (char*)file_ger->messages[i].msg.c_str();
		}
	}

	for (int i = 0; i < file_fra->messages.size(); i++)
	{
		sFraMessageEntryTablePtr[i].textId = file_fra->messages[i].id;
		sFraMessageEntryTablePtr[i].typePos = (file_fra->messages[i].textboxType << 4) | file_fra->messages[i].textboxYPos;
		sFraMessageEntryTablePtr[i].segment = file_fra->messages[i].msg.c_str();
		sFraMessageEntryTablePtr[i].msgSize = file_fra->messages[i].msg.size();

		if (file_fra->messages[i].id == 0xFFFC) {
			_message_0xFFFC_fra = (char*)file_fra->messages[i].msg.c_str();
		}
	}

	auto file2 = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource("text/staff_message_data_static/staff_message_data_static"));

	sStaffMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file2->messages.size());

	for (int i = 0; i < file2->messages.size(); i++)
	{
		sStaffMessageEntryTablePtr[i].textId = file2->messages[i].id;
		sStaffMessageEntryTablePtr[i].typePos = (file2->messages[i].textboxType << 4) | file2->messages[i].textboxYPos;
		sStaffMessageEntryTablePtr[i].segment = file2->messages[i].msg.c_str();
		sStaffMessageEntryTablePtr[i].msgSize = file2->messages[i].msg.size();
	}
}
