#include <cstdint>
#include <utility>
#include "../Game/GameSpecificFlashImpl.hpp"

void HandleGetAllResolutions(void* movie);
void HandleGetCurrResolution(void* movie);
void HandleSetCurrResolution(void* movie, float index);
void HandleGetCurrGraphicType(void* movie);
void HandleSetCurrGraphicType(void* movie, float vsync);
void HandleGetMenuOptionsList(void* movie);
void HandleSelectOption(void* movie);
void HandlePopupMessage(void* movie);
/*
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
*/
