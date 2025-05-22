#include <cstdint>
#include <utility>
#include "../Game/GameSpecificFlashImpl.hpp"

inline auto GameSpecificFlashImpl_SetFlashVariableFunc = (void(__thiscall*)(class GameSpecificFlashImpl*, const char*, char, void*))(0x01164570);

class OptionFlashCallbacks {
	class GetOptionsList : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SelectOption : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GoBack : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetOptionVal : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetOptionVal : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetOptionsListTypes : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetAllResolutions : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetCurrResolution : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetCurrResolution : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetCurrGraphicType : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetCurrGraphicType : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetCurrLayoutType : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetCurrLayoutType : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetCurrBrightness : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetCurrBrightness : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetKeyFromMapper : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class StartSetKeyInMapper : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetKeyInMapper : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class ResetKeysInMapper : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetTitle : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetItemName : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetAllItems : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class GetCurrItem : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetCurrItem : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class OnPageBack : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class UpdateTriggerByState : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class SetMenuId : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class PopupMessage : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };
	class IsInPause : public GameSpecificFlashFunction { public: OptionFlashCallbacks* callbacks_list = nullptr; virtual void operator()(void*, void*) override; };

	class FrontendFlashFunctions : public GameSpecificFlashImpl {
	public:
		OptionFlashCallbacks* callbacks_list;
	public:
		FrontendFlashFunctions();
		~FrontendFlashFunctions();
		virtual void SetupFlashFunctions();
	};

	static_assert(sizeof(FrontendFlashFunctions) == 64);

public:
	void* unk;
	void* parent_movie;
	bool unk2;
	FrontendFlashFunctions flash_functions;
	GetOptionsList get_options_list;
	SelectOption select_option;
	GoBack go_back;
	GetOptionVal get_option_val;
	SetOptionVal set_option_val;
	GetOptionsListTypes get_options_list_types;
	GetAllResolutions get_all_resolutions;
	GetCurrResolution get_curr_resolution;
	SetCurrResolution set_curr_resolution;
	GetCurrGraphicType get_curr_graphic_type;
	SetCurrGraphicType set_curr_graphic_type;
	GetCurrLayoutType get_curr_layout_type;
	SetCurrLayoutType set_curr_layout_type;
	GetCurrBrightness get_curr_brightness;
	SetCurrBrightness set_curr_brightness;
	GetKeyFromMapper get_key_from_mapper;
	StartSetKeyInMapper start_set_key_in_mapper;
	SetKeyInMapper set_key_in_mapper;
	ResetKeysInMapper reset_keys_in_mapper;
	GetTitle get_title;
	GetItemName get_item_name;
	GetAllItems get_all_items;
	GetCurrItem get_curr_item;
	SetCurrItem set_curr_item;
	OnPageBack on_page_back;
	UpdateTriggerByState update_trigger_by_state;
	SetMenuId set_menu_id;
	PopupMessage popup_message;
	IsInPause is_in_pause;
	void* unk3;
public:
	OptionFlashCallbacks();
	~OptionFlashCallbacks();
	void AttachLayer();
	void Init(void* _unk1);
};

static_assert(sizeof(OptionFlashCallbacks) == 0x138);

GameSpecificFlashImpl::GameSpecificFlashImpl()
{
}

GameSpecificFlashImpl::~GameSpecificFlashImpl()
{
}

void GameSpecificFlashImpl::SetFlashVariableFunc(const char* name, SetFlashVariDef::ArgumentType unk, GameSpecificFlashFunction* callback) {
	GameSpecificFlashImpl_SetFlashVariableFunc(this, name, std::to_underlying(unk), callback);
}

OptionFlashCallbacks::OptionFlashCallbacks()
{
}

OptionFlashCallbacks::~OptionFlashCallbacks()
{
}

void OptionFlashCallbacks::AttachLayer() {
	get_options_list.callbacks_list = this;
	select_option.callbacks_list = this;
	go_back.callbacks_list = this;
	get_option_val.callbacks_list = this;
	set_option_val.callbacks_list = this;
	get_options_list_types.callbacks_list = this;
	get_all_resolutions.callbacks_list = this;
	get_curr_resolution.callbacks_list = this;
	set_curr_resolution.callbacks_list = this;
	get_curr_graphic_type.callbacks_list = this;
	set_curr_graphic_type.callbacks_list = this;
	get_curr_layout_type.callbacks_list = this;
	set_curr_layout_type.callbacks_list = this;
	get_curr_brightness.callbacks_list = this;
	set_curr_brightness.callbacks_list = this;
	get_key_from_mapper.callbacks_list = this;
	start_set_key_in_mapper.callbacks_list = this;
	set_key_in_mapper.callbacks_list = this;
	reset_keys_in_mapper.callbacks_list = this;
	get_title.callbacks_list = this;
	get_item_name.callbacks_list = this;
	get_all_items.callbacks_list = this;
	get_curr_item.callbacks_list = this;
	set_curr_item.callbacks_list = this;
	on_page_back.callbacks_list = this;
	update_trigger_by_state.callbacks_list = this;
	set_menu_id.callbacks_list = this;
	popup_message.callbacks_list = this;
	is_in_pause.callbacks_list = this;
	flash_functions.SetupFlashFunctions();
}

void OptionFlashCallbacks::Init(void* movie) {
	if (movie != nullptr) {
		this->unk = nullptr;
		this->parent_movie = movie;
		AttachLayer();
	}
}

OptionFlashCallbacks::FrontendFlashFunctions::FrontendFlashFunctions() {
	callbacks_list = nullptr;
}

OptionFlashCallbacks::FrontendFlashFunctions::~FrontendFlashFunctions()
{
}

void OptionFlashCallbacks::FrontendFlashFunctions::SetupFlashFunctions() {
	SetFlashVariableFunc("GetMenuOptionsList", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_options_list);
	SetFlashVariableFunc("SelectOption", SetFlashVariDef::ArgumentType::String, &callbacks_list->select_option);
	SetFlashVariableFunc("GetOptionsListTypes", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_options_list_types);
	SetFlashVariableFunc("GetOptionVal", SetFlashVariDef::ArgumentType::String, &callbacks_list->get_option_val);
	SetFlashVariableFunc("SetOptionVal", SetFlashVariDef::ArgumentType::String, &callbacks_list->set_option_val);
	SetFlashVariableFunc("GoBack", SetFlashVariDef::ArgumentType::Null, &callbacks_list->go_back);
	SetFlashVariableFunc("GetAllResolutions", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_all_resolutions);
	SetFlashVariableFunc("GetCurrResolution", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_curr_resolution);
	SetFlashVariableFunc("SetCurrResolution", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_curr_resolution);
	SetFlashVariableFunc("GetCurrGraphicType", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_curr_graphic_type);
	SetFlashVariableFunc("SetCurrGraphicType", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_curr_graphic_type);
	SetFlashVariableFunc("GetCurrLayoutType", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_curr_layout_type);
	SetFlashVariableFunc("SetCurrLayoutType", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_curr_layout_type);
	SetFlashVariableFunc("GetCurrBrightness", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_curr_brightness);
	SetFlashVariableFunc("SetCurrBrightness", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_curr_brightness);
	SetFlashVariableFunc("GetKeyFromMapper", SetFlashVariDef::ArgumentType::Number, &callbacks_list->get_key_from_mapper);
	SetFlashVariableFunc("StartSetKeyInMapper", SetFlashVariDef::ArgumentType::Null, &callbacks_list->start_set_key_in_mapper);
	SetFlashVariableFunc("SetKeyInMapper", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_key_in_mapper);
	SetFlashVariableFunc("ResetKeysInMapper", SetFlashVariDef::ArgumentType::Null, &callbacks_list->reset_keys_in_mapper);
	SetFlashVariableFunc("GetTitle", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_title);
	SetFlashVariableFunc("GetItemName", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_item_name);
	SetFlashVariableFunc("GetAllItems", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_all_items);
	SetFlashVariableFunc("GetCurrItem", SetFlashVariDef::ArgumentType::Null, &callbacks_list->get_curr_item);
	SetFlashVariableFunc("SetCurrItem", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_curr_item);
	SetFlashVariableFunc("OnPageBack", SetFlashVariDef::ArgumentType::Null, &callbacks_list->on_page_back);
	SetFlashVariableFunc("UpdateTriggerByState", SetFlashVariDef::ArgumentType::Number, &callbacks_list->update_trigger_by_state);
	SetFlashVariableFunc("SetMenuId", SetFlashVariDef::ArgumentType::Number, &callbacks_list->set_menu_id);
	SetFlashVariableFunc("PopupMessage", SetFlashVariDef::ArgumentType::Number, &callbacks_list->popup_message);
	SetFlashVariableFunc("IsInPause", SetFlashVariDef::ArgumentType::Null, &callbacks_list->is_in_pause);
}

void OptionFlashCallbacks::GetOptionsList::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SelectOption::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GoBack::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetOptionVal::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetOptionVal::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetOptionsListTypes::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetAllResolutions::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetCurrResolution::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetCurrResolution::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetCurrGraphicType::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetCurrGraphicType::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetCurrLayoutType::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetCurrLayoutType::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetCurrBrightness::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetCurrBrightness::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetKeyFromMapper::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::StartSetKeyInMapper::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetKeyInMapper::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::ResetKeysInMapper::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetTitle::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetItemName::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetAllItems::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::GetCurrItem::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetCurrItem::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::OnPageBack::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::UpdateTriggerByState::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::SetMenuId::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::PopupMessage::operator()(void* val, void* movie) {}
void OptionFlashCallbacks::IsInPause::operator()(void* val, void* movie) {}

