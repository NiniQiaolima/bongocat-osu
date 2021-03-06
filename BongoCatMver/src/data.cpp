#include "header.hpp"

namespace data
{
Json::Value cfg;
std::map<std::string, sf::Texture> img_holder;
sf::Vector2i positionlast;
bool is_new_window=true;



void create_config()
{
    const char *s =
        R"V0G0N({
    "mode": 1,
    "decoration": {
        "window_size":[612,352],
        "leftHanded": false,
        "rgb": [255, 255, 255],
        "offsetX": [0, 11],
        "offsetY": [0, -65],
        "scalar": [1.0, 1.0],
		"framerateLimit":60,
		"topWindow": false,
		"armLineColor": [0, 0, 0],
        "hand_offset":[0,0],
		"emoticonKeep": false,
		"emoticonClear":[222],
        "soundKeep":true,
        "soundClear":[222],
        "correct":100,
        "l2d_correct":1.1,
        "l2d_horizontal_flip":false,
        "mouse_force_move":false,
        "desktop_pet":false
	},
    "workarea":{
        "workarea":false,
        "top_left":[0,0],
        "right_bottom":[1920,1080]
    },
	"standard":{
		"mouse":true,
		"mouse_left": [1],
		"mouse_right":[2],
         "mouse_side":[5],
		"keyboard":[[90], [88], [188],[190]],
		"hand":[[90],[88],[188],[190]],
		"face":[[186]],
        "sounds":[[90],[88],[188],[190]]
	},
	"keyboard":{
		"keyboard":[[90], [88], [188], [190]],
		"lefthand":[[87],[65],[83],[68]],
		"righthand":[[17,40],[17,37],[17,39],[17,38]],
		"face":[[97],[98],[99],[100]],
        "sounds":[[90],[88],[188],[190]]
	}

})V0G0N";
    std::string error;
    Json::CharReaderBuilder cfg_builder;
    Json::CharReader *cfg_reader = cfg_builder.newCharReader();
    cfg_reader->parse(s, s + strlen(s), &cfg, &error);
}

void error_msg(std::string error, std::string title)
{
    if (MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_RETRYCANCEL) == IDCANCEL)
        exit(0);
}

bool update(Json::Value &cfg_default, Json::Value &cfg)
{
    bool is_update = true;
    for (const auto &key : cfg.getMemberNames())
        if (cfg_default.isMember(key))
        {
            if (cfg_default[key].type() != cfg[key].type())
            {
                error_msg("Value type error in config.json", "Error reading configs");
                return false;
            }
            if (cfg_default[key].isArray())
                for (Json::Value &v : cfg[key])
                    if (v.type() != cfg_default[key][0].type())
                    {
                        error_msg("Value type error in config.json", "Error reading configs");
                        return false;
                    }
            if (cfg_default[key].isObject())
                is_update &= update(cfg_default[key], cfg[key]);
            else
                cfg_default[key] = cfg[key];
        }
    return is_update;
}

bool init()
{
    //��ȡjson
    while (true)
    {
        create_config();
        std::ifstream cfg_file("config.json", std::ifstream::binary);
        if (!cfg_file.good())
            break;
        std::string cfg_string((std::istreambuf_iterator<char>(cfg_file)), std::istreambuf_iterator<char>()), error;
        Json::CharReaderBuilder cfg_builder;
        Json::CharReader *cfg_reader = cfg_builder.newCharReader();
        Json::Value cfg_read;
        if (!cfg_reader->parse(cfg_string.c_str(), cfg_string.c_str() + cfg_string.size(), &cfg_read, &error))
            error_msg("Syntax error in config.json:\n" + error, "Error reading configs");
        else if (update(cfg, cfg_read))
            break;
    }
    return true;
}

sf::Texture &load_texture(std::string path)
{
    if (img_holder.find(path) == img_holder.end())
        while (!img_holder[path].loadFromFile(path))
            error_msg("Cannot find file " + path, "Error importing images");
    img_holder[path].setSmooth(true);
    return img_holder[path];
}

sf::Texture &load_texture2(std::string path)
{
    if (img_holder.find(path) == img_holder.end()) {
        img_holder[path].loadFromFile(path);
        img_holder[path].setSmooth(true);
    }
	return img_holder[path];
}
}; // namespace data
