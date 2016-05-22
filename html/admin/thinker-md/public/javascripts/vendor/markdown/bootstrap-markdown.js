'use strict';

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol ? "symbol" : typeof obj; };

/* ===================================================
 * bootstrap-markdown.js v2.8.0
 * http://github.com/toopay/bootstrap-markdown
 * ===================================================
 * Copyright 2013-2014 Taufan Aditya
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================== */

!function ($) {

    "use strict"; // jshint ;_;

    /* MARKDOWN CLASS DEFINITION
     * ========================== */

    var Markdown = function Markdown(element, options) {
        // @TODO : remove this BC on next major release
        // @see : https://github.com/toopay/bootstrap-markdown/issues/109
        var opts = ['autofocus', 'savable', 'hideable', 'width', 'height', 'resize', 'iconlibrary', 'language', 'imgurl', 'base64url', 'localStorage', 'footer', 'fullscreen', 'hiddenButtons', 'disabledButtons', 'flowChart'];
        $.each(opts, function (_, opt) {
            if (typeof $(element).data(opt) !== 'undefined') {
                options = (typeof options === 'undefined' ? 'undefined' : _typeof(options)) == 'object' ? options : {};
                options[opt] = $(element).data(opt);
            }
        });
        // End BC

        //emoji
        this.$emoji = {
            groupNav: { 'github-emoji': null, 'twemoji': null, 'font-awesome': null },
            groupName: ['github-emoji', 'twemoji', 'font-awesome'],
            groupPanel: { 'github-emoji': null, 'twemoji': null, 'font-awesome': null },
            groups: {
                "github-emoji": {
                    "People": ["bowtie", "smile", "laughing", "blush", "smiley", "relaxed", "smirk", "heart_eyes", "kissing_heart", "kissing_closed_eyes", "flushed", "relieved", "satisfied", "grin", "wink", "stuck_out_tongue_winking_eye", "stuck_out_tongue_closed_eyes", "grinning", "kissing", "kissing_smiling_eyes", "stuck_out_tongue", "sleeping", "worried", "frowning", "anguished", "open_mouth", "grimacing", "confused", "hushed", "expressionless", "unamused", "sweat_smile", "sweat", "disappointed_relieved", "weary", "pensive", "disappointed", "confounded", "fearful", "cold_sweat", "persevere", "cry", "sob", "joy", "astonished", "scream", "neckbeard", "tired_face", "angry", "rage", "triumph", "sleepy", "yum", "mask", "sunglasses", "dizzy_face", "imp", "smiling_imp", "neutral_face", "no_mouth", "innocent", "alien", "yellow_heart", "blue_heart", "purple_heart", "heart", "green_heart", "broken_heart", "heartbeat", "heartpulse", "two_hearts", "revolving_hearts", "cupid", "sparkling_heart", "sparkles", "star", "star2", "dizzy", "boom", "collision", "anger", "exclamation", "question", "grey_exclamation", "grey_question", "zzz", "dash", "sweat_drops", "notes", "musical_note", "fire", "hankey", "poop", "shit", "+1", "thumbsup", "-1", "thumbsdown", "ok_hand", "punch", "facepunch", "fist", "v", "wave", "hand", "raised_hand", "open_hands", "point_up", "point_down", "point_left", "point_right", "raised_hands", "pray", "point_up_2", "clap", "muscle", "metal", "fu", "walking", "runner", "running", "couple", "family", "two_men_holding_hands", "two_women_holding_hands", "dancer", "dancers", "ok_woman", "no_good", "information_desk_person", "raising_hand", "bride_with_veil", "person_with_pouting_face", "person_frowning", "bow", "couplekiss", "couple_with_heart", "massage", "haircut", "nail_care", "boy", "girl", "woman", "man", "baby", "older_woman", "older_man", "person_with_blond_hair", "man_with_gua_pi_mao", "man_with_turban", "construction_worker", "cop", "angel", "princess", "smiley_cat", "smile_cat", "heart_eyes_cat", "kissing_cat", "smirk_cat", "scream_cat", "crying_cat_face", "joy_cat", "pouting_cat", "japanese_ogre", "japanese_goblin", "see_no_evil", "hear_no_evil", "speak_no_evil", "guardsman", "skull", "feet", "lips", "kiss", "droplet", "ear", "eyes", "nose", "tongue", "love_letter", "bust_in_silhouette", "busts_in_silhouette", "speech_balloon", "thought_balloon", "feelsgood", "finnadie", "goberserk", "godmode", "hurtrealbad", "rage1", "rage2", "rage3", "rage4", "suspect", "trollface"],
                    "Nature": ["sunny", "umbrella", "cloud", "snowflake", "snowman", "zap", "cyclone", "foggy", "ocean", "cat", "dog", "mouse", "hamster", "rabbit", "wolf", "frog", "tiger", "koala", "bear", "pig", "pig_nose", "cow", "boar", "monkey_face", "monkey", "horse", "racehorse", "camel", "sheep", "elephant", "panda_face", "snake", "bird", "baby_chick", "hatched_chick", "hatching_chick", "chicken", "penguin", "turtle", "bug", "honeybee", "ant", "beetle", "snail", "octopus", "tropical_fish", "fish", "whale", "whale2", "dolphin", "cow2", "ram", "rat", "water_buffalo", "tiger2", "rabbit2", "dragon", "goat", "rooster", "dog2", "pig2", "mouse2", "ox", "dragon_face", "blowfish", "crocodile", "dromedary_camel", "leopard", "cat2", "poodle", "paw_prints", "bouquet", "cherry_blossom", "tulip", "four_leaf_clover", "rose", "sunflower", "hibiscus", "maple_leaf", "leaves", "fallen_leaf", "herb", "mushroom", "cactus", "palm_tree", "evergreen_tree", "deciduous_tree", "chestnut", "seedling", "blossom", "ear_of_rice", "shell", "globe_with_meridians", "sun_with_face", "full_moon_with_face", "new_moon_with_face", "new_moon", "waxing_crescent_moon", "first_quarter_moon", "waxing_gibbous_moon", "full_moon", "waning_gibbous_moon", "last_quarter_moon", "waning_crescent_moon", "last_quarter_moon_with_face", "first_quarter_moon_with_face", "moon", "earth_africa", "earth_americas", "earth_asia", "volcano", "milky_way", "partly_sunny", "octocat", "squirrel"],
                    "Objects": ["bamboo", "gift_heart", "dolls", "school_satchel", "mortar_board", "flags", "fireworks", "sparkler", "wind_chime", "rice_scene", "jack_o_lantern", "ghost", "santa", "christmas_tree", "gift", "bell", "no_bell", "tanabata_tree", "tada", "confetti_ball", "balloon", "crystal_ball", "cd", "dvd", "floppy_disk", "camera", "video_camera", "movie_camera", "computer", "tv", "iphone", "phone", "telephone", "telephone_receiver", "pager", "fax", "minidisc", "vhs", "sound", "speaker", "mute", "loudspeaker", "mega", "hourglass", "hourglass_flowing_sand", "alarm_clock", "watch", "radio", "satellite", "loop", "mag", "mag_right", "unlock", "lock", "lock_with_ink_pen", "closed_lock_with_key", "key", "bulb", "flashlight", "high_brightness", "low_brightness", "electric_plug", "battery", "calling", "email", "mailbox", "postbox", "bath", "bathtub", "shower", "toilet", "wrench", "nut_and_bolt", "hammer", "seat", "moneybag", "yen", "dollar", "pound", "euro", "credit_card", "money_with_wings", "e-mail", "inbox_tray", "outbox_tray", "envelope", "incoming_envelope", "postal_horn", "mailbox_closed", "mailbox_with_mail", "mailbox_with_no_mail", "package", "door", "smoking", "bomb", "gun", "hocho", "pill", "syringe", "page_facing_up", "page_with_curl", "bookmark_tabs", "bar_chart", "chart_with_upwards_trend", "chart_with_downwards_trend", "scroll", "clipboard", "calendar", "date", "card_index", "file_folder", "open_file_folder", "scissors", "pushpin", "paperclip", "black_nib", "pencil2", "straight_ruler", "triangular_ruler", "closed_book", "green_book", "blue_book", "orange_book", "notebook", "notebook_with_decorative_cover", "ledger", "books", "bookmark", "name_badge", "microscope", "telescope", "newspaper", "football", "basketball", "soccer", "baseball", "tennis", "8ball", "rugby_football", "bowling", "golf", "mountain_bicyclist", "bicyclist", "horse_racing", "snowboarder", "swimmer", "surfer", "ski", "spades", "hearts", "clubs", "diamonds", "gem", "ring", "trophy", "musical_score", "musical_keyboard", "violin", "space_invader", "video_game", "black_joker", "flower_playing_cards", "game_die", "dart", "mahjong", "clapper", "memo", "pencil", "book", "art", "microphone", "headphones", "trumpet", "saxophone", "guitar", "shoe", "sandal", "high_heel", "lipstick", "boot", "shirt", "tshirt", "necktie", "womans_clothes", "dress", "running_shirt_with_sash", "jeans", "kimono", "bikini", "ribbon", "tophat", "crown", "womans_hat", "mans_shoe", "closed_umbrella", "briefcase", "handbag", "pouch", "purse", "eyeglasses", "fishing_pole_and_fish", "coffee", "tea", "sake", "baby_bottle", "beer", "beers", "cocktail", "tropical_drink", "wine_glass", "fork_and_knife", "pizza", "hamburger", "fries", "poultry_leg", "meat_on_bone", "spaghetti", "curry", "fried_shrimp", "bento", "sushi", "fish_cake", "rice_ball", "rice_cracker", "rice", "ramen", "stew", "oden", "dango", "egg", "bread", "doughnut", "custard", "icecream", "ice_cream", "shaved_ice", "birthday", "cake", "cookie", "chocolate_bar", "candy", "lollipop", "honey_pot", "apple", "green_apple", "tangerine", "lemon", "cherries", "grapes", "watermelon", "strawberry", "peach", "melon", "banana", "pear", "pineapple", "sweet_potato", "eggplant", "tomato", "corn"],
                    "Places": ["house", "house_with_garden", "school", "office", "post_office", "hospital", "bank", "convenience_store", "love_hotel", "hotel", "wedding", "church", "department_store", "european_post_office", "city_sunrise", "city_sunset", "japanese_castle", "european_castle", "tent", "factory", "tokyo_tower", "japan", "mount_fuji", "sunrise_over_mountains", "sunrise", "stars", "statue_of_liberty", "bridge_at_night", "carousel_horse", "rainbow", "ferris_wheel", "fountain", "roller_coaster", "ship", "speedboat", "boat", "sailboat", "rowboat", "anchor", "rocket", "airplane", "helicopter", "steam_locomotive", "tram", "mountain_railway", "bike", "aerial_tramway", "suspension_railway", "mountain_cableway", "tractor", "blue_car", "oncoming_automobile", "car", "red_car", "taxi", "oncoming_taxi", "articulated_lorry", "bus", "oncoming_bus", "rotating_light", "police_car", "oncoming_police_car", "fire_engine", "ambulance", "minibus", "truck", "train", "station", "train2", "bullettrain_front", "bullettrain_side", "light_rail", "monorail", "railway_car", "trolleybus", "ticket", "fuelpump", "vertical_traffic_light", "traffic_light", "warning", "construction", "beginner", "atm", "slot_machine", "busstop", "barber", "hotsprings", "checkered_flag", "crossed_flags", "izakaya_lantern", "moyai", "circus_tent", "performing_arts", "round_pushpin", "triangular_flag_on_post", "jp", "kr", "cn", "us", "fr", "es", "it", "ru", "gb", "uk", "de"],
                    "Symbols": ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "keycap_ten", "1234", "zero", "hash", "symbols", "arrow_backward", "arrow_down", "arrow_forward", "arrow_left", "capital_abcd", "abcd", "abc", "arrow_lower_left", "arrow_lower_right", "arrow_right", "arrow_up", "arrow_upper_left", "arrow_upper_right", "arrow_double_down", "arrow_double_up", "arrow_down_small", "arrow_heading_down", "arrow_heading_up", "leftwards_arrow_with_hook", "arrow_right_hook", "left_right_arrow", "arrow_up_down", "arrow_up_small", "arrows_clockwise", "arrows_counterclockwise", "rewind", "fast_forward", "information_source", "ok", "twisted_rightwards_arrows", "repeat", "repeat_one", "new", "top", "up", "cool", "free", "ng", "cinema", "koko", "signal_strength", "u5272", "u5408", "u55b6", "u6307", "u6708", "u6709", "u6e80", "u7121", "u7533", "u7a7a", "u7981", "sa", "restroom", "mens", "womens", "baby_symbol", "no_smoking", "parking", "wheelchair", "metro", "baggage_claim", "accept", "wc", "potable_water", "put_litter_in_its_place", "secret", "congratulations", "m", "passport_control", "left_luggage", "customs", "ideograph_advantage", "cl", "sos", "id", "no_entry_sign", "underage", "no_mobile_phones", "do_not_litter", "non-potable_water", "no_bicycles", "no_pedestrians", "children_crossing", "no_entry", "eight_spoked_asterisk", "sparkle", "eight_pointed_black_star", "heart_decoration", "vs", "vibration_mode", "mobile_phone_off", "chart", "currency_exchange", "aries", "taurus", "gemini", "cancer", "leo", "virgo", "libra", "scorpius", "sagittarius", "capricorn", "aquarius", "pisces", "ophiuchus", "six_pointed_star", "negative_squared_cross_mark", "a", "b", "ab", "o2", "diamond_shape_with_a_dot_inside", "recycle", "end", "back", "on", "soon", "clock1", "clock130", "clock10", "clock1030", "clock11", "clock1130", "clock12", "clock1230", "clock2", "clock230", "clock3", "clock330", "clock4", "clock430", "clock5", "clock530", "clock6", "clock630", "clock7", "clock730", "clock8", "clock830", "clock9", "clock930", "heavy_dollar_sign", "copyright", "registered", "tm", "x", "heavy_exclamation_mark", "bangbang", "interrobang", "o", "heavy_multiplication_x", "heavy_plus_sign", "heavy_minus_sign", "heavy_division_sign", "white_flower", "100", "heavy_check_mark", "ballot_box_with_check", "radio_button", "link", "curly_loop", "wavy_dash", "part_alternation_mark", "trident", "black_small_square", "white_small_square", "black_medium_small_square", "white_medium_small_square", "black_medium_square", "white_medium_square", "black_large_square", "white_large_square", "white_check_mark", "black_square_button", "white_square_button", "black_circle", "white_circle", "red_circle", "large_blue_circle", "large_blue_diamond", "large_orange_diamond", "small_blue_diamond", "small_orange_diamond", "small_red_triangle", "small_red_triangle_down", "shipit"]
                },
                "twemoji": ["tw-1f004", "tw-1f0cf", "tw-1f170", "tw-1f171", "tw-1f17e", "tw-1f17f", "tw-1f18e", "tw-1f191", "tw-1f192", "tw-1f193", "tw-1f194", "tw-1f195", "tw-1f196", "tw-1f197", "tw-1f198", "tw-1f199", "tw-1f19a", "tw-1f1e6", "tw-1f1e7", "tw-1f1e8-1f1f3", "tw-1f1e8", "tw-1f1e9-1f1ea", "tw-1f1e9", "tw-1f1ea-1f1f8", "tw-1f1ea", "tw-1f1eb-1f1f7", "tw-1f1eb", "tw-1f1ec-1f1e7", "tw-1f1ec", "tw-1f1ed", "tw-1f1ee-1f1f9", "tw-1f1ee", "tw-1f1ef-1f1f5", "tw-1f1ef", "tw-1f1f0-1f1f7", "tw-1f1f0", "tw-1f1f1", "tw-1f1f2", "tw-1f1f3", "tw-1f1f4", "tw-1f1f5", "tw-1f1f6", "tw-1f1f7-1f1fa", "tw-1f1f7", "tw-1f1f8", "tw-1f1f9", "tw-1f1fa-1f1f8", "tw-1f1fa", "tw-1f1fb", "tw-1f1fc", "tw-1f1fd", "tw-1f1fe", "tw-1f1ff", "tw-1f201", "tw-1f202", "tw-1f21a", "tw-1f22f", "tw-1f232", "tw-1f233", "tw-1f234", "tw-1f235", "tw-1f236", "tw-1f237", "tw-1f238", "tw-1f239", "tw-1f23a", "tw-1f250", "tw-1f251", "tw-1f300", "tw-1f301", "tw-1f302", "tw-1f303", "tw-1f304", "tw-1f305", "tw-1f306", "tw-1f307", "tw-1f308", "tw-1f309", "tw-1f30a", "tw-1f30b", "tw-1f30c", "tw-1f30d", "tw-1f30e", "tw-1f30f", "tw-1f310", "tw-1f311", "tw-1f312", "tw-1f313", "tw-1f314", "tw-1f315", "tw-1f316", "tw-1f317", "tw-1f318", "tw-1f319", "tw-1f31a", "tw-1f31b", "tw-1f31c", "tw-1f31d", "tw-1f31e", "tw-1f31f", "tw-1f320", "tw-1f330", "tw-1f331", "tw-1f332", "tw-1f333", "tw-1f334", "tw-1f335", "tw-1f337", "tw-1f338", "tw-1f339", "tw-1f33a", "tw-1f33b", "tw-1f33c", "tw-1f33d", "tw-1f33e", "tw-1f33f", "tw-1f340", "tw-1f341", "tw-1f342", "tw-1f343", "tw-1f344", "tw-1f345", "tw-1f346", "tw-1f347", "tw-1f348", "tw-1f349", "tw-1f34a", "tw-1f34b", "tw-1f34c", "tw-1f34d", "tw-1f34e", "tw-1f34f", "tw-1f350", "tw-1f351", "tw-1f352", "tw-1f353", "tw-1f354", "tw-1f355", "tw-1f356", "tw-1f357", "tw-1f358", "tw-1f359", "tw-1f35a", "tw-1f35b", "tw-1f35c", "tw-1f35d", "tw-1f35e", "tw-1f35f", "tw-1f360", "tw-1f361", "tw-1f362", "tw-1f363", "tw-1f364", "tw-1f365", "tw-1f366", "tw-1f367", "tw-1f368", "tw-1f369", "tw-1f36a", "tw-1f36b", "tw-1f36c", "tw-1f36d", "tw-1f36e", "tw-1f36f", "tw-1f370", "tw-1f371", "tw-1f372", "tw-1f373", "tw-1f374", "tw-1f375", "tw-1f376", "tw-1f377", "tw-1f378", "tw-1f379", "tw-1f37a", "tw-1f37b", "tw-1f37c", "tw-1f380", "tw-1f381", "tw-1f382", "tw-1f383", "tw-1f384", "tw-1f385", "tw-1f386", "tw-1f387", "tw-1f388", "tw-1f389", "tw-1f38a", "tw-1f38b", "tw-1f38c", "tw-1f38d", "tw-1f38e", "tw-1f38f", "tw-1f390", "tw-1f391", "tw-1f392", "tw-1f393", "tw-1f3a0", "tw-1f3a1", "tw-1f3a2", "tw-1f3a3", "tw-1f3a4", "tw-1f3a5", "tw-1f3a6", "tw-1f3a7", "tw-1f3a8", "tw-1f3a9", "tw-1f3aa", "tw-1f3ab", "tw-1f3ac", "tw-1f3ad", "tw-1f3ae", "tw-1f3af", "tw-1f3b0", "tw-1f3b1", "tw-1f3b2", "tw-1f3b3", "tw-1f3b4", "tw-1f3b5", "tw-1f3b6", "tw-1f3b7", "tw-1f3b8", "tw-1f3b9", "tw-1f3ba", "tw-1f3bb", "tw-1f3bc", "tw-1f3bd", "tw-1f3be", "tw-1f3bf", "tw-1f3c0", "tw-1f3c1", "tw-1f3c2", "tw-1f3c3", "tw-1f3c4", "tw-1f3c6", "tw-1f3c7", "tw-1f3c8", "tw-1f3c9", "tw-1f3ca", "tw-1f3e0", "tw-1f3e1", "tw-1f3e2", "tw-1f3e3", "tw-1f3e4", "tw-1f3e5", "tw-1f3e6", "tw-1f3e7", "tw-1f3e8", "tw-1f3e9", "tw-1f3ea", "tw-1f3eb", "tw-1f3ec", "tw-1f3ed", "tw-1f3ee", "tw-1f3ef", "tw-1f3f0", "tw-1f400", "tw-1f401", "tw-1f402", "tw-1f403", "tw-1f404", "tw-1f405", "tw-1f406", "tw-1f407", "tw-1f408", "tw-1f409", "tw-1f40a", "tw-1f40b", "tw-1f40c", "tw-1f40d", "tw-1f40e", "tw-1f40f", "tw-1f410", "tw-1f411", "tw-1f412", "tw-1f413", "tw-1f414", "tw-1f415", "tw-1f416", "tw-1f417", "tw-1f418", "tw-1f419", "tw-1f41a", "tw-1f41b", "tw-1f41c", "tw-1f41d", "tw-1f41e", "tw-1f41f", "tw-1f420", "tw-1f421", "tw-1f422", "tw-1f423", "tw-1f424", "tw-1f425", "tw-1f426", "tw-1f427", "tw-1f428", "tw-1f429", "tw-1f42a", "tw-1f42b", "tw-1f42c", "tw-1f42d", "tw-1f42e", "tw-1f42f", "tw-1f430", "tw-1f431", "tw-1f432", "tw-1f433", "tw-1f434", "tw-1f435", "tw-1f436", "tw-1f437", "tw-1f438", "tw-1f439", "tw-1f43a", "tw-1f43b", "tw-1f43c", "tw-1f43d", "tw-1f43e", "tw-1f440", "tw-1f442", "tw-1f443", "tw-1f444", "tw-1f445", "tw-1f446", "tw-1f447", "tw-1f448", "tw-1f449", "tw-1f44a", "tw-1f44b", "tw-1f44c", "tw-1f44d", "tw-1f44e", "tw-1f44f", "tw-1f450", "tw-1f451", "tw-1f452", "tw-1f453", "tw-1f454", "tw-1f455", "tw-1f456", "tw-1f457", "tw-1f458", "tw-1f459", "tw-1f45a", "tw-1f45b", "tw-1f45c", "tw-1f45d", "tw-1f45e", "tw-1f45f", "tw-1f460", "tw-1f461", "tw-1f462", "tw-1f463", "tw-1f464", "tw-1f465", "tw-1f466", "tw-1f467", "tw-1f468", "tw-1f469", "tw-1f46a", "tw-1f46b", "tw-1f46c", "tw-1f46d", "tw-1f46e", "tw-1f46f", "tw-1f470", "tw-1f471", "tw-1f472", "tw-1f473", "tw-1f474", "tw-1f475", "tw-1f476", "tw-1f477", "tw-1f478", "tw-1f479", "tw-1f47a", "tw-1f47b", "tw-1f47c", "tw-1f47d", "tw-1f47e", "tw-1f47f", "tw-1f480", "tw-1f481", "tw-1f482", "tw-1f483", "tw-1f484", "tw-1f485", "tw-1f486", "tw-1f487", "tw-1f488", "tw-1f489", "tw-1f48a", "tw-1f48b", "tw-1f48c", "tw-1f48d", "tw-1f48e", "tw-1f48f", "tw-1f490", "tw-1f491", "tw-1f492", "tw-1f493", "tw-1f494", "tw-1f495", "tw-1f496", "tw-1f497", "tw-1f498", "tw-1f499", "tw-1f49a", "tw-1f49b", "tw-1f49c", "tw-1f49d", "tw-1f49e", "tw-1f49f", "tw-1f4a0", "tw-1f4a1", "tw-1f4a2", "tw-1f4a3", "tw-1f4a4", "tw-1f4a5", "tw-1f4a6", "tw-1f4a7", "tw-1f4a8", "tw-1f4a9", "tw-1f4aa", "tw-1f4ab", "tw-1f4ac", "tw-1f4ad", "tw-1f4ae", "tw-1f4af", "tw-1f4b0", "tw-1f4b1", "tw-1f4b2", "tw-1f4b3", "tw-1f4b4", "tw-1f4b5", "tw-1f4b6", "tw-1f4b7", "tw-1f4b8", "tw-1f4b9", "tw-1f4ba", "tw-1f4bb", "tw-1f4bc", "tw-1f4bd", "tw-1f4be", "tw-1f4bf", "tw-1f4c0", "tw-1f4c1", "tw-1f4c2", "tw-1f4c3", "tw-1f4c4", "tw-1f4c5", "tw-1f4c6", "tw-1f4c7", "tw-1f4c8", "tw-1f4c9", "tw-1f4ca", "tw-1f4cb", "tw-1f4cc", "tw-1f4cd", "tw-1f4ce", "tw-1f4cf", "tw-1f4d0", "tw-1f4d1", "tw-1f4d2", "tw-1f4d3", "tw-1f4d4", "tw-1f4d5", "tw-1f4d6", "tw-1f4d7", "tw-1f4d8", "tw-1f4d9", "tw-1f4da", "tw-1f4db", "tw-1f4dc", "tw-1f4dd", "tw-1f4de", "tw-1f4df", "tw-1f4e0", "tw-1f4e1", "tw-1f4e2", "tw-1f4e3", "tw-1f4e4", "tw-1f4e5", "tw-1f4e6", "tw-1f4e7", "tw-1f4e8", "tw-1f4e9", "tw-1f4ea", "tw-1f4eb", "tw-1f4ec", "tw-1f4ed", "tw-1f4ee", "tw-1f4ef", "tw-1f4f0", "tw-1f4f1", "tw-1f4f2", "tw-1f4f3", "tw-1f4f4", "tw-1f4f5", "tw-1f4f6", "tw-1f4f7", "tw-1f4f9", "tw-1f4fa", "tw-1f4fb", "tw-1f4fc", "tw-1f500", "tw-1f501", "tw-1f502", "tw-1f503", "tw-1f504", "tw-1f505", "tw-1f506", "tw-1f507", "tw-1f508", "tw-1f509", "tw-1f50a", "tw-1f50b", "tw-1f50c", "tw-1f50d", "tw-1f50e", "tw-1f50f", "tw-1f510", "tw-1f511", "tw-1f512", "tw-1f513", "tw-1f514", "tw-1f515", "tw-1f516", "tw-1f517", "tw-1f518", "tw-1f519", "tw-1f51a", "tw-1f51b", "tw-1f51c", "tw-1f51d", "tw-1f51e", "tw-1f51f", "tw-1f520", "tw-1f521", "tw-1f522", "tw-1f523", "tw-1f524", "tw-1f525", "tw-1f526", "tw-1f527", "tw-1f528", "tw-1f529", "tw-1f52a", "tw-1f52b", "tw-1f52c", "tw-1f52d", "tw-1f52e", "tw-1f52f", "tw-1f530", "tw-1f531", "tw-1f532", "tw-1f533", "tw-1f534", "tw-1f535", "tw-1f536", "tw-1f537", "tw-1f538", "tw-1f539", "tw-1f53a", "tw-1f53b", "tw-1f53c", "tw-1f53d", "tw-1f550", "tw-1f551", "tw-1f552", "tw-1f553", "tw-1f554", "tw-1f555", "tw-1f556", "tw-1f557", "tw-1f558", "tw-1f559", "tw-1f55a", "tw-1f55b", "tw-1f55c", "tw-1f55d", "tw-1f55e", "tw-1f55f", "tw-1f560", "tw-1f561", "tw-1f562", "tw-1f563", "tw-1f564", "tw-1f565", "tw-1f566", "tw-1f567", "tw-1f5fb", "tw-1f5fc", "tw-1f5fd", "tw-1f5fe", "tw-1f5ff", "tw-1f600", "tw-1f601", "tw-1f602", "tw-1f603", "tw-1f604", "tw-1f605", "tw-1f606", "tw-1f607", "tw-1f608", "tw-1f609", "tw-1f60a", "tw-1f60b", "tw-1f60c", "tw-1f60d", "tw-1f60e", "tw-1f60f", "tw-1f610", "tw-1f611", "tw-1f612", "tw-1f613", "tw-1f614", "tw-1f615", "tw-1f616", "tw-1f617", "tw-1f618", "tw-1f619", "tw-1f61a", "tw-1f61b", "tw-1f61c", "tw-1f61d", "tw-1f61e", "tw-1f61f", "tw-1f620", "tw-1f621", "tw-1f622", "tw-1f623", "tw-1f624", "tw-1f625", "tw-1f626", "tw-1f627", "tw-1f628", "tw-1f629", "tw-1f62a", "tw-1f62b", "tw-1f62c", "tw-1f62d", "tw-1f62e", "tw-1f62f", "tw-1f630", "tw-1f631", "tw-1f632", "tw-1f633", "tw-1f634", "tw-1f635", "tw-1f636", "tw-1f637", "tw-1f638", "tw-1f639", "tw-1f63a", "tw-1f63b", "tw-1f63c", "tw-1f63d", "tw-1f63e", "tw-1f63f", "tw-1f640", "tw-1f645", "tw-1f646", "tw-1f647", "tw-1f648", "tw-1f649", "tw-1f64a", "tw-1f64b", "tw-1f64c", "tw-1f64d", "tw-1f64e", "tw-1f64f", "tw-1f680", "tw-1f681", "tw-1f682", "tw-1f683", "tw-1f684", "tw-1f685", "tw-1f686", "tw-1f687", "tw-1f688", "tw-1f689", "tw-1f68a", "tw-1f68b", "tw-1f68c", "tw-1f68d", "tw-1f68e", "tw-1f68f", "tw-1f690", "tw-1f691", "tw-1f692", "tw-1f693", "tw-1f694", "tw-1f695", "tw-1f696", "tw-1f697", "tw-1f698", "tw-1f699", "tw-1f69a", "tw-1f69b", "tw-1f69c", "tw-1f69d", "tw-1f69e", "tw-1f69f", "tw-1f6a0", "tw-1f6a1", "tw-1f6a2", "tw-1f6a3", "tw-1f6a4", "tw-1f6a5", "tw-1f6a6", "tw-1f6a7", "tw-1f6a8", "tw-1f6a9", "tw-1f6aa", "tw-1f6ab", "tw-1f6ac", "tw-1f6ad", "tw-1f6ae", "tw-1f6af", "tw-1f6b0", "tw-1f6b1", "tw-1f6b2", "tw-1f6b3", "tw-1f6b4", "tw-1f6b5", "tw-1f6b6", "tw-1f6b7", "tw-1f6b8", "tw-1f6b9", "tw-1f6ba", "tw-1f6bb", "tw-1f6bc", "tw-1f6bd", "tw-1f6be", "tw-1f6bf", "tw-1f6c0", "tw-1f6c1", "tw-1f6c2", "tw-1f6c3", "tw-1f6c4", "tw-1f6c5", "tw-203c", "tw-2049", "tw-2122", "tw-2139", "tw-2194", "tw-2195", "tw-2196", "tw-2197", "tw-2198", "tw-2199", "tw-21a9", "tw-21aa", "tw-23-20e3", "tw-231a", "tw-231b", "tw-23e9", "tw-23ea", "tw-23eb", "tw-23ec", "tw-23f0", "tw-23f3", "tw-24c2", "tw-25aa", "tw-25ab", "tw-25b6", "tw-25c0", "tw-25fb", "tw-25fc", "tw-25fd", "tw-25fe", "tw-2600", "tw-2601", "tw-260e", "tw-2611", "tw-2614", "tw-2615", "tw-261d", "tw-263a", "tw-2648", "tw-2649", "tw-264a", "tw-264b", "tw-264c", "tw-264d", "tw-264e", "tw-264f", "tw-2650", "tw-2651", "tw-2652", "tw-2653", "tw-2660", "tw-2663", "tw-2665", "tw-2666", "tw-2668", "tw-267b", "tw-267f", "tw-2693", "tw-26a0", "tw-26a1", "tw-26aa", "tw-26ab", "tw-26bd", "tw-26be", "tw-26c4", "tw-26c5", "tw-26ce", "tw-26d4", "tw-26ea", "tw-26f2", "tw-26f3", "tw-26f5", "tw-26fa", "tw-26fd", "tw-2702", "tw-2705", "tw-2708", "tw-2709", "tw-270a", "tw-270b", "tw-270c", "tw-270f", "tw-2712", "tw-2714", "tw-2716", "tw-2728", "tw-2733", "tw-2734", "tw-2744", "tw-2747", "tw-274c", "tw-274e", "tw-2753", "tw-2754", "tw-2755", "tw-2757", "tw-2764", "tw-2795", "tw-2796", "tw-2797", "tw-27a1", "tw-27b0", "tw-27bf", "tw-2934", "tw-2935", "tw-2b05", "tw-2b06", "tw-2b07", "tw-2b1b", "tw-2b1c", "tw-2b50", "tw-2b55", "tw-30-20e3", "tw-3030", "tw-303d", "tw-31-20e3", "tw-32-20e3", "tw-3297", "tw-3299", "tw-33-20e3", "tw-34-20e3", "tw-35-20e3", "tw-36-20e3", "tw-37-20e3", "tw-38-20e3", "tw-39-20e3", "tw-a9", "tw-ae", "tw-e50a"],
                "font-awesome": ["fa-glass", "fa-music", "fa-search", "fa-envelope-o", "fa-heart", "fa-star", "fa-star-o", "fa-user", "fa-film", "fa-th-large", "fa-th", "fa-th-list", "fa-check", "fa-times", "fa-search-plus", "fa-search-minus", "fa-power-off", "fa-signal", "fa-cog", "fa-trash-o", "fa-home", "fa-file-o", "fa-clock-o", "fa-road", "fa-download", "fa-arrow-circle-o-down", "fa-arrow-circle-o-up", "fa-inbox", "fa-play-circle-o", "fa-repeat", "fa-refresh", "fa-list-alt", "fa-lock", "fa-flag", "fa-headphones", "fa-volume-off", "fa-volume-down", "fa-volume-up", "fa-qrcode", "fa-barcode", "fa-tag", "fa-tags", "fa-book", "fa-bookmark", "fa-print", "fa-camera", "fa-font", "fa-bold", "fa-italic", "fa-text-height", "fa-text-width", "fa-align-left", "fa-align-center", "fa-align-right", "fa-align-justify", "fa-list", "fa-outdent", "fa-indent", "fa-video-camera", "fa-picture-o", "fa-pencil", "fa-map-marker", "fa-adjust", "fa-tint", "fa-pencil-square-o", "fa-share-square-o", "fa-check-square-o", "fa-arrows", "fa-step-backward", "fa-fast-backward", "fa-backward", "fa-play", "fa-pause", "fa-stop", "fa-forward", "fa-fast-forward", "fa-step-forward", "fa-eject", "fa-chevron-left", "fa-chevron-right", "fa-plus-circle", "fa-minus-circle", "fa-times-circle", "fa-check-circle", "fa-question-circle", "fa-info-circle", "fa-crosshairs", "fa-times-circle-o", "fa-check-circle-o", "fa-ban", "fa-arrow-left", "fa-arrow-right", "fa-arrow-up", "fa-arrow-down", "fa-share", "fa-expand", "fa-compress", "fa-plus", "fa-minus", "fa-asterisk", "fa-exclamation-circle", "fa-gift", "fa-leaf", "fa-fire", "fa-eye", "fa-eye-slash", "fa-exclamation-triangle", "fa-plane", "fa-calendar", "fa-random", "fa-comment", "fa-magnet", "fa-chevron-up", "fa-chevron-down", "fa-retweet", "fa-shopping-cart", "fa-folder", "fa-folder-open", "fa-arrows-v", "fa-arrows-h", "fa-bar-chart", "fa-twitter-square", "fa-facebook-square", "fa-camera-retro", "fa-key", "fa-cogs", "fa-comments", "fa-thumbs-o-up", "fa-thumbs-o-down", "fa-star-half", "fa-heart-o", "fa-sign-out", "fa-linkedin-square", "fa-thumb-tack", "fa-external-link", "fa-sign-in", "fa-trophy", "fa-github-square", "fa-upload", "fa-lemon-o", "fa-phone", "fa-square-o", "fa-bookmark-o", "fa-phone-square", "fa-twitter", "fa-facebook", "fa-github", "fa-unlock", "fa-credit-card", "fa-rss", "fa-hdd-o", "fa-bullhorn", "fa-bell", "fa-certificate", "fa-hand-o-right", "fa-hand-o-left", "fa-hand-o-up", "fa-hand-o-down", "fa-arrow-circle-left", "fa-arrow-circle-right", "fa-arrow-circle-up", "fa-arrow-circle-down", "fa-globe", "fa-wrench", "fa-tasks", "fa-filter", "fa-briefcase", "fa-arrows-alt", "fa-users", "fa-link", "fa-cloud", "fa-flask", "fa-scissors", "fa-files-o", "fa-paperclip", "fa-floppy-o", "fa-square", "fa-bars", "fa-list-ul", "fa-list-ol", "fa-strikethrough", "fa-underline", "fa-table", "fa-magic", "fa-truck", "fa-pinterest", "fa-pinterest-square", "fa-google-plus-square", "fa-google-plus", "fa-money", "fa-caret-down", "fa-caret-up", "fa-caret-left", "fa-caret-right", "fa-columns", "fa-sort", "fa-sort-desc", "fa-sort-asc", "fa-envelope", "fa-linkedin", "fa-undo", "fa-gavel", "fa-tachometer", "fa-comment-o", "fa-comments-o", "fa-bolt", "fa-sitemap", "fa-umbrella", "fa-clipboard", "fa-lightbulb-o", "fa-exchange", "fa-cloud-download", "fa-cloud-upload", "fa-user-md", "fa-stethoscope", "fa-suitcase", "fa-bell-o", "fa-coffee", "fa-cutlery", "fa-file-text-o", "fa-building-o", "fa-hospital-o", "fa-ambulance", "fa-medkit", "fa-fighter-jet", "fa-beer", "fa-h-square", "fa-plus-square", "fa-angle-double-left", "fa-angle-double-right", "fa-angle-double-up", "fa-angle-double-down", "fa-angle-left", "fa-angle-right", "fa-angle-up", "fa-angle-down", "fa-desktop", "fa-laptop", "fa-tablet", "fa-mobile", "fa-circle-o", "fa-quote-left", "fa-quote-right", "fa-spinner", "fa-circle", "fa-reply", "fa-github-alt", "fa-folder-o", "fa-folder-open-o", "fa-smile-o", "fa-frown-o", "fa-meh-o", "fa-gamepad", "fa-keyboard-o", "fa-flag-o", "fa-flag-checkered", "fa-terminal", "fa-code", "fa-reply-all", "fa-star-half-o", "fa-location-arrow", "fa-crop", "fa-code-fork", "fa-chain-broken", "fa-question", "fa-info", "fa-exclamation", "fa-superscript", "fa-subscript", "fa-eraser", "fa-puzzle-piece", "fa-microphone", "fa-microphone-slash", "fa-shield", "fa-calendar-o", "fa-fire-extinguisher", "fa-rocket", "fa-maxcdn", "fa-chevron-circle-left", "fa-chevron-circle-right", "fa-chevron-circle-up", "fa-chevron-circle-down", "fa-html5", "fa-css3", "fa-anchor", "fa-unlock-alt", "fa-bullseye", "fa-ellipsis-h", "fa-ellipsis-v", "fa-rss-square", "fa-play-circle", "fa-ticket", "fa-minus-square", "fa-minus-square-o", "fa-level-up", "fa-level-down", "fa-check-square", "fa-pencil-square", "fa-share-square", "fa-compass", "fa-caret-square-o-down", "fa-caret-square-o-up", "fa-caret-square-o-right", "fa-eur", "fa-gbp", "fa-usd", "fa-inr", "fa-jpy", "fa-rub", "fa-krw", "fa-btc", "fa-file", "fa-file-text", "fa-sort-alpha-asc", "fa-sort-alpha-desc", "fa-sort-amount-asc", "fa-sort-amount-desc", "fa-sort-numeric-asc", "fa-sort-numeric-desc", "fa-thumbs-up", "fa-thumbs-down", "fa-youtube-square", "fa-youtube", "fa-xing", "fa-xing-square", "fa-youtube-play", "fa-dropbox", "fa-stack-overflow", "fa-instagram", "fa-flickr", "fa-adn", "fa-bitbucket", "fa-bitbucket-square", "fa-tumblr", "fa-tumblr-square", "fa-long-arrow-down", "fa-long-arrow-up", "fa-long-arrow-left", "fa-long-arrow-right", "fa-apple", "fa-windows", "fa-android", "fa-linux", "fa-dribbble", "fa-skype", "fa-foursquare", "fa-trello", "fa-female", "fa-male", "fa-gratipay", "fa-sun-o", "fa-moon-o", "fa-archive", "fa-bug", "fa-vk", "fa-weibo", "fa-renren", "fa-pagelines", "fa-stack-exchange", "fa-arrow-circle-o-right", "fa-arrow-circle-o-left", "fa-caret-square-o-left", "fa-dot-circle-o", "fa-wheelchair", "fa-vimeo-square", "fa-try", "fa-plus-square-o", "fa-space-shuttle", "fa-slack", "fa-envelope-square", "fa-wordpress", "fa-openid", "fa-university", "fa-graduation-cap", "fa-yahoo", "fa-google", "fa-reddit", "fa-reddit-square", "fa-stumbleupon-circle", "fa-stumbleupon", "fa-delicious", "fa-digg", "fa-pied-piper", "fa-pied-piper-alt", "fa-drupal", "fa-joomla", "fa-language", "fa-fax", "fa-building", "fa-child", "fa-paw", "fa-spoon", "fa-cube", "fa-cubes", "fa-behance", "fa-behance-square", "fa-steam", "fa-steam-square", "fa-recycle", "fa-car", "fa-taxi", "fa-tree", "fa-spotify", "fa-deviantart", "fa-soundcloud", "fa-database", "fa-file-pdf-o", "fa-file-word-o", "fa-file-excel-o", "fa-file-powerpoint-o", "fa-file-image-o", "fa-file-archive-o", "fa-file-audio-o", "fa-file-video-o", "fa-file-code-o", "fa-vine", "fa-codepen", "fa-jsfiddle", "fa-life-ring", "fa-circle-o-notch", "fa-rebel", "fa-empire", "fa-git-square", "fa-git", "fa-hacker-news", "fa-tencent-weibo", "fa-qq", "fa-weixin", "fa-paper-plane", "fa-paper-plane-o", "fa-history", "fa-circle-thin", "fa-header", "fa-paragraph", "fa-sliders", "fa-share-alt", "fa-share-alt-square", "fa-bomb", "fa-futbol-o", "fa-tty", "fa-binoculars", "fa-plug", "fa-slideshare", "fa-twitch", "fa-yelp", "fa-newspaper-o", "fa-wifi", "fa-calculator", "fa-paypal", "fa-google-wallet", "fa-cc-visa", "fa-cc-mastercard", "fa-cc-discover", "fa-cc-amex", "fa-cc-paypal", "fa-cc-stripe", "fa-bell-slash", "fa-bell-slash-o", "fa-trash", "fa-copyright", "fa-at", "fa-eyedropper", "fa-paint-brush", "fa-birthday-cake", "fa-area-chart", "fa-pie-chart", "fa-line-chart", "fa-lastfm", "fa-lastfm-square", "fa-toggle-off", "fa-toggle-on", "fa-bicycle", "fa-bus", "fa-ioxhost", "fa-angellist", "fa-cc", "fa-ils", "fa-meanpath", "fa-buysellads", "fa-connectdevelop", "fa-dashcube", "fa-forumbee", "fa-leanpub", "fa-sellsy", "fa-shirtsinbulk", "fa-simplybuilt", "fa-skyatlas", "fa-cart-plus", "fa-cart-arrow-down", "fa-diamond", "fa-ship", "fa-user-secret", "fa-motorcycle", "fa-street-view", "fa-heartbeat", "fa-venus", "fa-mars", "fa-mercury", "fa-transgender", "fa-transgender-alt", "fa-venus-double", "fa-mars-double", "fa-venus-mars", "fa-mars-stroke", "fa-mars-stroke-v", "fa-mars-stroke-h", "fa-neuter", "fa-facebook-official", "fa-pinterest-p", "fa-whatsapp", "fa-server", "fa-user-plus", "fa-user-times", "fa-bed", "fa-viacoin", "fa-train", "fa-subway", "fa-medium", "fa-GitHub", "fa-bed", "fa-buysellads", "fa-cart-arrow-down", "fa-cart-plus", "fa-connectdevelop", "fa-dashcube", "fa-diamond", "fa-facebook-official", "fa-forumbee", "fa-heartbeat", "fa-hotel", "fa-leanpub", "fa-mars", "fa-mars-double", "fa-mars-stroke", "fa-mars-stroke-h", "fa-mars-stroke-v", "fa-medium", "fa-mercury", "fa-motorcycle", "fa-neuter", "fa-pinterest-p", "fa-sellsy", "fa-server", "fa-ship", "fa-shirtsinbulk", "fa-simplybuilt", "fa-skyatlas", "fa-street-view", "fa-subway", "fa-train", "fa-transgender", "fa-transgender-alt", "fa-user-plus", "fa-user-secret", "fa-user-times", "fa-venus", "fa-venus-double", "fa-venus-mars", "fa-viacoin", "fa-whatsapp", "fa-adjust", "fa-anchor", "fa-archive", "fa-area-chart", "fa-arrows", "fa-arrows-h", "fa-arrows-v", "fa-asterisk", "fa-at", "fa-automobile", "fa-ban", "fa-bank", "fa-bar-chart", "fa-bar-chart-o", "fa-barcode", "fa-bars", "fa-bed", "fa-beer", "fa-bell", "fa-bell-o", "fa-bell-slash", "fa-bell-slash-o", "fa-bicycle", "fa-binoculars", "fa-birthday-cake", "fa-bolt", "fa-bomb", "fa-book", "fa-bookmark", "fa-bookmark-o", "fa-briefcase", "fa-bug", "fa-building", "fa-building-o", "fa-bullhorn", "fa-bullseye", "fa-bus", "fa-cab", "fa-calculator", "fa-calendar", "fa-calendar-o", "fa-camera", "fa-camera-retro", "fa-car", "fa-caret-square-o-down", "fa-caret-square-o-left", "fa-caret-square-o-right", "fa-caret-square-o-up", "fa-cart-arrow-down", "fa-cart-plus", "fa-cc", "fa-certificate", "fa-check", "fa-check-circle", "fa-check-circle-o", "fa-check-square", "fa-check-square-o", "fa-child", "fa-circle", "fa-circle-o", "fa-circle-o-notch", "fa-circle-thin", "fa-clock-o", "fa-close", "fa-cloud", "fa-cloud-download", "fa-cloud-upload", "fa-code", "fa-code-fork", "fa-coffee", "fa-cog", "fa-cogs", "fa-comment", "fa-comment-o", "fa-comments", "fa-comments-o", "fa-compass", "fa-copyright", "fa-credit-card", "fa-crop", "fa-crosshairs", "fa-cube", "fa-cubes", "fa-cutlery", "fa-dashboard", "fa-database", "fa-desktop", "fa-diamond", "fa-dot-circle-o", "fa-download", "fa-edit", "fa-ellipsis-h", "fa-ellipsis-v", "fa-envelope", "fa-envelope-o", "fa-envelope-square", "fa-eraser", "fa-exchange", "fa-exclamation", "fa-exclamation-circle", "fa-exclamation-triangle", "fa-external-link", "fa-external-link-square", "fa-eye", "fa-eye-slash", "fa-eyedropper", "fa-fax", "fa-female", "fa-fighter-jet", "fa-file-archive-o", "fa-file-audio-o", "fa-file-code-o", "fa-file-excel-o", "fa-file-image-o", "fa-file-movie-o", "fa-file-pdf-o", "fa-file-photo-o", "fa-file-picture-o", "fa-file-powerpoint-o", "fa-file-sound-o", "fa-file-video-o", "fa-file-word-o", "fa-file-zip-o", "fa-film", "fa-filter", "fa-fire", "fa-fire-extinguisher", "fa-flag", "fa-flag-checkered", "fa-flag-o", "fa-flash", "fa-flask", "fa-folder", "fa-folder-o", "fa-folder-open", "fa-folder-open-o", "fa-frown-o", "fa-futbol-o", "fa-gamepad", "fa-gavel", "fa-gear", "fa-gears", "fa-genderless", "fa-gift", "fa-glass", "fa-globe", "fa-graduation-cap", "fa-group", "fa-hdd-o", "fa-headphones", "fa-heart", "fa-heart-o", "fa-heartbeat", "fa-history", "fa-home", "fa-hotel", "fa-image", "fa-inbox", "fa-info", "fa-info-circle", "fa-institution", "fa-key", "fa-keyboard-o", "fa-language", "fa-laptop", "fa-leaf", "fa-legal", "fa-lemon-o", "fa-level-down", "fa-level-up", "fa-life-bouy", "fa-life-buoy", "fa-life-ring", "fa-life-saver", "fa-lightbulb-o", "fa-line-chart", "fa-location-arrow", "fa-lock", "fa-magic", "fa-magnet", "fa-mail-forward", "fa-mail-reply", "fa-mail-reply-all", "fa-male", "fa-map-marker", "fa-meh-o", "fa-microphone", "fa-microphone-slash", "fa-minus", "fa-minus-circle", "fa-minus-square", "fa-minus-square-o", "fa-mobile", "fa-mobile-phone", "fa-money", "fa-moon-o", "fa-mortar-board", "fa-motorcycle", "fa-music", "fa-navicon", "fa-newspaper-o", "fa-paint-brush", "fa-paper-plane", "fa-paper-plane-o", "fa-paw", "fa-pencil", "fa-pencil-square", "fa-pencil-square-o", "fa-phone", "fa-phone-square", "fa-photo", "fa-picture-o", "fa-pie-chart", "fa-plane", "fa-plug", "fa-plus", "fa-plus-circle", "fa-plus-square", "fa-plus-square-o", "fa-power-off", "fa-print", "fa-puzzle-piece", "fa-qrcode", "fa-question", "fa-question-circle", "fa-quote-left", "fa-quote-right", "fa-random", "fa-recycle", "fa-refresh", "fa-remove", "fa-reorder", "fa-reply", "fa-reply-all", "fa-retweet", "fa-road", "fa-rocket", "fa-rss", "fa-rss-square", "fa-search", "fa-search-minus", "fa-search-plus", "fa-send", "fa-send-o", "fa-server", "fa-share", "fa-share-alt", "fa-share-alt-square", "fa-share-square", "fa-share-square-o", "fa-shield", "fa-ship", "fa-shopping-cart", "fa-sign-in", "fa-sign-out", "fa-signal", "fa-sitemap", "fa-sliders", "fa-smile-o", "fa-soccer-ball-o", "fa-sort", "fa-sort-alpha-asc", "fa-sort-alpha-desc", "fa-sort-amount-asc", "fa-sort-amount-desc", "fa-sort-asc", "fa-sort-desc", "fa-sort-down", "fa-sort-numeric-asc", "fa-sort-numeric-desc", "fa-sort-up", "fa-space-shuttle", "fa-spinner", "fa-spoon", "fa-square", "fa-square-o", "fa-star", "fa-star-half", "fa-star-half-empty", "fa-star-half-full", "fa-star-half-o", "fa-star-o", "fa-street-view", "fa-suitcase", "fa-sun-o", "fa-support", "fa-tablet", "fa-tachometer", "fa-tag", "fa-tags", "fa-tasks", "fa-taxi", "fa-terminal", "fa-thumb-tack", "fa-thumbs-down", "fa-thumbs-o-down", "fa-thumbs-o-up", "fa-thumbs-up", "fa-ticket", "fa-times", "fa-times-circle", "fa-times-circle-o", "fa-tint", "fa-toggle-down", "fa-toggle-left", "fa-toggle-off", "fa-toggle-on", "fa-toggle-right", "fa-toggle-up", "fa-trash", "fa-trash-o", "fa-tree", "fa-trophy", "fa-truck", "fa-tty", "fa-umbrella", "fa-university", "fa-unlock", "fa-unlock-alt", "fa-unsorted", "fa-upload", "fa-user", "fa-user-plus", "fa-user-secret", "fa-user-times", "fa-users", "fa-video-camera", "fa-volume-down", "fa-volume-off", "fa-volume-up", "fa-warning", "fa-wheelchair", "fa-wifi", "fa-wrench", "fa-ambulance", "fa-automobile", "fa-bicycle", "fa-bus", "fa-cab", "fa-car", "fa-fighter-jet", "fa-motorcycle", "fa-plane", "fa-rocket", "fa-ship", "fa-space-shuttle", "fa-subway", "fa-taxi", "fa-train", "fa-truck", "fa-wheelchair", "fa-circle-thin", "fa-genderless", "fa-mars", "fa-mars-double", "fa-mars-stroke", "fa-mars-stroke-h", "fa-mars-stroke-v", "fa-mercury", "fa-neuter", "fa-transgender", "fa-transgender-alt", "fa-venus", "fa-venus-double", "fa-venus-mars", "fa-file", "fa-file-archive-o", "fa-file-audio-o", "fa-file-code-o", "fa-file-excel-o", "fa-file-image-o", "fa-file-movie-o", "fa-file-o", "fa-file-pdf-o", "fa-file-photo-o", "fa-file-picture-o", "fa-file-powerpoint-o", "fa-file-sound-o", "fa-file-text", "fa-file-text-o", "fa-file-video-o", "fa-file-word-o", "fa-file-zip-o", "fa-circle-o-notch", "fa-cog", "fa-gear", "fa-refresh", "fa-spinner", "fa-check-square", "fa-check-square-o", "fa-circle", "fa-circle-o", "fa-dot-circle-o", "fa-minus-square", "fa-minus-square-o", "fa-plus-square", "fa-plus-square-o", "fa-square", "fa-square-o", "fa-cc-amex", "fa-cc-discover", "fa-cc-mastercard", "fa-cc-paypal", "fa-cc-stripe", "fa-cc-visa", "fa-credit-card", "fa-google-wallet", "fa-paypal", "fa-area-chart", "fa-bar-chart", "fa-bar-chart-o", "fa-line-chart", "fa-pie-chart", "fa-bitcoin", "fa-btc", "fa-cny", "fa-dollar", "fa-eur", "fa-euro", "fa-gbp", "fa-ils", "fa-inr", "fa-jpy", "fa-krw", "fa-money", "fa-rmb", "fa-rouble", "fa-rub", "fa-ruble", "fa-rupee", "fa-shekel", "fa-sheqel", "fa-try", "fa-turkish-lira", "fa-usd", "fa-won", "fa-yen", "fa-align-center", "fa-align-justify", "fa-align-left", "fa-align-right", "fa-bold", "fa-chain", "fa-chain-broken", "fa-clipboard", "fa-columns", "fa-copy", "fa-cut", "fa-dedent", "fa-eraser", "fa-file", "fa-file-o", "fa-file-text", "fa-file-text-o", "fa-files-o", "fa-floppy-o", "fa-font", "fa-header", "fa-indent", "fa-italic", "fa-link", "fa-list", "fa-list-alt", "fa-list-ol", "fa-list-ul", "fa-outdent", "fa-paperclip", "fa-paragraph", "fa-paste", "fa-repeat", "fa-rotate-left", "fa-rotate-right", "fa-save", "fa-scissors", "fa-strikethrough", "fa-subscript", "fa-superscript", "fa-table", "fa-text-height", "fa-text-width", "fa-th", "fa-th-large", "fa-th-list", "fa-underline", "fa-undo", "fa-unlink", "fa-angle-double-down", "fa-angle-double-left", "fa-angle-double-right", "fa-angle-double-up", "fa-angle-down", "fa-angle-left", "fa-angle-right", "fa-angle-up", "fa-arrow-circle-down", "fa-arrow-circle-left", "fa-arrow-circle-o-down", "fa-arrow-circle-o-left", "fa-arrow-circle-o-right", "fa-arrow-circle-o-up", "fa-arrow-circle-right", "fa-arrow-circle-up", "fa-arrow-down", "fa-arrow-left", "fa-arrow-right", "fa-arrow-up", "fa-arrows", "fa-arrows-alt", "fa-arrows-h", "fa-arrows-v", "fa-caret-down", "fa-caret-left", "fa-caret-right", "fa-caret-square-o-down", "fa-caret-square-o-left", "fa-caret-square-o-right", "fa-caret-square-o-up", "fa-caret-up", "fa-chevron-circle-down", "fa-chevron-circle-left", "fa-chevron-circle-right", "fa-chevron-circle-up", "fa-chevron-down", "fa-chevron-left", "fa-chevron-right", "fa-chevron-up", "fa-hand-o-down", "fa-hand-o-left", "fa-hand-o-right", "fa-hand-o-up", "fa-long-arrow-down", "fa-long-arrow-left", "fa-long-arrow-right", "fa-long-arrow-up", "fa-toggle-down", "fa-toggle-left", "fa-toggle-right", "fa-toggle-up", "fa-arrows-alt", "fa-backward", "fa-compress", "fa-eject", "fa-expand", "fa-fast-backward", "fa-fast-forward", "fa-forward", "fa-pause", "fa-play", "fa-play-circle", "fa-play-circle-o", "fa-step-backward", "fa-step-forward", "fa-stop", "fa-youtube-play", "fa-report", "fa-adn", "fa-android", "fa-angellist", "fa-apple", "fa-behance", "fa-behance-square", "fa-bitbucket", "fa-bitbucket-square", "fa-bitcoin", "fa-btc", "fa-buysellads", "fa-cc-amex", "fa-cc-discover", "fa-cc-mastercard", "fa-cc-paypal", "fa-cc-stripe", "fa-cc-visa", "fa-codepen", "fa-connectdevelop", "fa-css3", "fa-dashcube", "fa-delicious", "fa-deviantart", "fa-digg", "fa-dribbble", "fa-dropbox", "fa-drupal", "fa-empire", "fa-facebook", "fa-facebook-f", "fa-facebook-official", "fa-facebook-square", "fa-flickr", "fa-forumbee", "fa-foursquare", "fa-ge", "fa-git", "fa-git-square", "fa-github", "fa-github-alt", "fa-github-square", "fa-gittip", "fa-google", "fa-google-plus", "fa-google-plus-square", "fa-google-wallet", "fa-gratipay", "fa-hacker-news", "fa-html5", "fa-instagram", "fa-ioxhost", "fa-joomla", "fa-jsfiddle", "fa-lastfm", "fa-lastfm-square", "fa-leanpub", "fa-linkedin", "fa-linkedin-square", "fa-linux", "fa-maxcdn", "fa-meanpath", "fa-medium", "fa-openid", "fa-pagelines", "fa-paypal", "fa-pied-piper", "fa-pied-piper-alt", "fa-pinterest", "fa-pinterest-p", "fa-pinterest-square", "fa-qq", "fa-ra", "fa-rebel", "fa-reddit", "fa-reddit-square", "fa-renren", "fa-sellsy", "fa-share-alt", "fa-share-alt-square", "fa-shirtsinbulk", "fa-simplybuilt", "fa-skyatlas", "fa-skype", "fa-slack", "fa-slideshare", "fa-soundcloud", "fa-spotify", "fa-stack-exchange", "fa-stack-overflow", "fa-steam", "fa-steam-square", "fa-stumbleupon", "fa-stumbleupon-circle", "fa-tencent-weibo", "fa-trello", "fa-tumblr", "fa-tumblr-square", "fa-twitch", "fa-twitter", "fa-twitter-square", "fa-viacoin", "fa-vimeo-square", "fa-vine", "fa-vk", "fa-wechat", "fa-weibo", "fa-weixin", "fa-whatsapp", "fa-windows", "fa-wordpress", "fa-xing", "fa-xing-square", "fa-yahoo", "fa-yelp", "fa-youtube", "fa-youtube-play", "fa-youtube-square", "fa-ambulance", "fa-h-square", "fa-heart", "fa-heart-o", "fa-heartbeat", "fa-hospital-o", "fa-medkit", "fa-plus-square", "fa-stethoscope", "fa-user-md", "fa-wheelchair"]
            }
        };

        // Class Properties
        this.$ns = 'bootstrap-markdown';
        this.$element = $(element);
        this.$editable = { el: null, type: null, attrKeys: [], attrValues: [], content: null };
        this.$options = $.extend(true, {}, $.fn.markdown.defaults, options, this.$element.data('options'));
        this.$oldContent = null;
        this.$isPreview = false;
        this.$isFullscreen = false;
        this.$editor = null;
        //add by wpl show markdown preview
        this.$fullscreenControls = false;
        //id
        this.$localStorage = options.localStorage;

        this.$uploadMode = false;
        this.$fullPreview = null;
        this.$innerPreview = null;
        this.$uploadPanel = null;
        this.$inputFile = null;
        this.$stateBar = null;
        this.$cutPaste = null;
        //上传进度条
        this.$progress = null;
        this.$percent = null;
        //上传文件限制512kb
        this.$fileSize = 524288;
        //registe
        this.$registPaste = false;
        //end
        this.$textarea = null;
        this.$handler = [];
        this.$callback = [];
        this.$nextTab = [];

        this.$emojiPanel = null;
        this.$emojiElements = null;

        this.isIE = navigator.appName == "Microsoft Internet Explorer";
        this.isIE8 = this.isIE && navigator.appVersion.match(/8./i) == "8.";

        this.showEditor();
    };

    Markdown.prototype = {

        constructor: Markdown,

        __alterButtons: function __alterButtons(name, alter) {
            var handler = this.$handler,
                isAll = name == 'all',
                that = this;

            $.each(handler, function (k, v) {
                var halt = true;
                if (isAll) {
                    halt = false;
                } else {
                    halt = v.indexOf(name) < 0;
                }

                if (halt === false) {
                    alter(that.$editor.find('button[data-handler="' + v + '"]'));
                }
            });
        },

        __buildButtons: function __buildButtons(buttonsArray, container) {
            var i,
                ns = this.$ns,
                handler = this.$handler,
                callback = this.$callback;

            for (i = 0; i < buttonsArray.length; i++) {
                // Build each group container
                var y,
                    btnGroups = buttonsArray[i];
                for (y = 0; y < btnGroups.length; y++) {
                    // Build each button group
                    var z,
                        buttons = btnGroups[y].data,
                        btnGroupContainer = $('<div/>', {
                        'class': 'btn-group'
                    });

                    for (z = 0; z < buttons.length; z++) {
                        var button = buttons[z],
                            buttonContainer,
                            buttonIconContainer,
                            buttonHandler = ns + '-' + button.name,
                            buttonIcon = this.__getIcon(button.icon),
                            btnText = button.btnText ? button.btnText : '',

                        //btnClass = button.btnClass ? button.btnClass : 'btn',
                        tabIndex = button.tabIndex ? button.tabIndex : '-1',
                            hotkey = typeof button.hotkey !== 'undefined' ? button.hotkey : '',
                            hotkeyCaption = typeof jQuery.hotkeys !== 'undefined' && hotkey !== '' ? ' (' + hotkey + ')' : '';

                        // Construct the button object
                        buttonContainer = $('<button></button>');
                        buttonContainer.attr({
                            'type': 'button',
                            'title': this.__localize(button.title) + hotkeyCaption,
                            'tabindex': tabIndex,
                            'data-provider': ns,
                            'data-handler': buttonHandler,
                            'data-hotkey': hotkey
                        });
                        if (button.toggle === true) {
                            buttonContainer.attr('data-toggle', 'button');
                        }
                        buttonIconContainer = $('<span/>');
                        buttonIconContainer.addClass(buttonIcon);
                        buttonIconContainer.prependTo(buttonContainer);

                        // Attach the button object
                        btnGroupContainer.append(buttonContainer);

                        // Register handler and callback
                        handler.push(buttonHandler);
                        callback.push(button.callback);
                    }

                    // Attach the button group into container dom
                    container.append(btnGroupContainer);
                }
            }

            return container;
        },
        __setListener: function __setListener() {
            // Set size and resizable Properties
            var hasRows = typeof this.$textarea.attr('rows') !== 'undefined',
                maxRows = this.$textarea.val().split("\n").length > 5 ? this.$textarea.val().split("\n").length : '5',
                rowsVal = hasRows ? this.$textarea.attr('rows') : maxRows,
                _this = this;

            this.$textarea.attr('rows', rowsVal);
            if (this.$options.resize) {
                this.$textarea.css('resize', this.$options.resize);
            }

            this.$textarea.on('focus', $.proxy(this.focus, this)).on('keypress', $.proxy(this.keypress, this)).on('keydown', function () {
                // $.proxy can't stop event
                _this.keydown.apply(_this, arguments);
            }).on('keyup', function () {
                _this.keyup.apply(_this, arguments);
            }).on('change', $.proxy(this.change, this));

            if (this.eventSupported('keydown')) {
                this.$textarea.on('keydown', $.proxy(this.keydown, this));
            }

            if (this.$options.fullscreen.enable) {
                this.$textarea.keyup(_.debounce(function () {
                    if (_this.$isFullscreen) {
                        _this.$innerPreview.html(_this.parseContent(_this.$textarea.val()));
                        _this.showDiagrams(_this.$innerPreview);
                    }
                }, _this.$options.fullscreen.debounce));

                this.$textarea.scroll(function () {
                    var __this = $(this).get(0),
                        scrollHeight = __this.scrollHeight,
                        scrollTop = __this.scrollTop;
                    var __inner = _this.$innerPreview.get(0),
                        innerHeight = __inner.scrollHeight;
                    var top = scrollTop * innerHeight / scrollHeight;
                    _this.$innerPreview.scrollTop(top);
                });
            }

            // Re-attach markdown data
            this.$textarea.data('markdown', this);
        },

        __handle: function __handle(e) {
            var target = $(e.currentTarget),
                handler = this.$handler,
                callback = this.$callback,
                handlerName = target.attr('data-handler'),
                callbackIndex = handler.indexOf(handlerName),
                callbackHandler = callback[callbackIndex];

            // Trigger the focusin
            $(e.currentTarget).focus();

            callbackHandler(this);

            // Trigger onChange for each button handle
            this.change(this);

            // Unless it was the save handler,
            // focusin the textarea
            if (handlerName.indexOf('cmdSave') < 0) {
                this.$textarea.focus();
            }

            e.preventDefault();
        },

        __localize: function __localize(string) {
            var messages = $.fn.markdown.messages,
                language = this.$options.language;
            if (typeof messages !== 'undefined' && typeof messages[language] !== 'undefined' && typeof messages[language][string] !== 'undefined') {
                return messages[language][string];
            }
            return string;
        },

        __getIcon: function __getIcon(src) {
            return (typeof src === 'undefined' ? 'undefined' : _typeof(src)) == 'object' ? src[this.$options.iconlibrary] : src;
        },

        setFullscreen: function setFullscreen(mode) {
            var $editor = this.$editor,
                $textarea = this.$textarea,
                $innerPreview = this.$innerPreview,

            //小预览窗口
            preview = $('div[data-provider="markdown-preview"]'),

            //预览按钮
            previewButton = $('button[data-handler="bootstrap-markdown-cmdPreview"]'),
                _this = this;
            if (mode) {
                if (this.$isPreview) {
                    this.hidePreview();
                }
                $editor.addClass('md-fullscreen-mode');
                $('body').addClass('md-nooverflow');
                this.$options.onFullscreen(this);

                $innerPreview.html(_this.parseContent($textarea.val()));
                this.showDiagrams($innerPreview);
                //up by wpl
                if (preview) {
                    preview.remove();
                }
                if (previewButton.length) {
                    previewButton.hide();
                }
            } else {
                $editor.removeClass('md-fullscreen-mode');
                $('body').removeClass('md-nooverflow');
                //up by wpl
                if (previewButton) {
                    previewButton.show();
                }
                if (this.$isPreview) {
                    this.showPreview();
                }
            }

            this.$isFullscreen = mode;
            $textarea.focus();
        }, showEditor: function showEditor() {

            var instance = this,
                textarea,
                ns = this.$ns,
                container = this.$element,
                originalHeigth = container.css('height'),
                originalWidth = container.css('width'),
                editable = this.$editable,
                handler = this.$handler,
                callback = this.$callback,
                editorId = this.$editorId,
                options = this.$options,
                _fullPreview = this.$fullPreview,
                innerPreview = this.$fullPreview,
                cutPaste = this.$cutPaste,
                editor = $('<div/>', {
                'class': 'md-editor',
                click: function click() {
                    instance.focus();
                }
            });

            // Prepare the editor
            if (this.$editor === null) {
                // Create the panel
                var editorHeader = $('<div/>', {
                    'class': 'md-header btn-toolbar'
                });

                // Merge the main & additional button groups together
                var allBtnGroups = [];
                if (options.buttons.length > 0) allBtnGroups = allBtnGroups.concat(options.buttons[0]);
                if (options.additionalButtons.length > 0) allBtnGroups = allBtnGroups.concat(options.additionalButtons[0]);

                // Reduce and/or reorder the button groups
                if (options.reorderButtonGroups.length > 0) {
                    allBtnGroups = allBtnGroups.filter(function (btnGroup) {
                        return options.reorderButtonGroups.indexOf(btnGroup.name) > -1;
                    }).sort(function (a, b) {
                        if (options.reorderButtonGroups.indexOf(a.name) < options.reorderButtonGroups.indexOf(b.name)) return -1;
                        if (options.reorderButtonGroups.indexOf(a.name) > options.reorderButtonGroups.indexOf(b.name)) return 1;
                        return 0;
                    });
                }

                // Build the buttons
                if (allBtnGroups.length > 0) {
                    editorHeader = this.__buildButtons([allBtnGroups], editorHeader);
                }

                if (options.fullscreen.enable) {
                    editorHeader.append('<div class="md-controls"><a class="md-control md-control-fullscreen" href="#"><span class="' + this.__getIcon(options.fullscreen.icons.fullscreenOn) + '"></span></a></div>').on('click', '.md-control-fullscreen', function (e) {
                        e.preventDefault();
                        instance.setFullscreen(true);
                    });
                }

                editor.append(editorHeader);

                var _localCache = '',
                    _localStorage = this.$localStorage;
                if (window.localStorage && _localStorage && '' !== _localStorage) {
                    _localCache = localStorage.getItem(_localStorage);
                }

                // Wrap the textarea
                if (container.is('textarea')) {
                    container.before(editor);
                    textarea = container;
                    textarea.addClass('md-input');
                    editor.append(textarea);
                } else {
                    // This is some arbitrary content that could be edited
                    textarea = $('<textarea/>', {
                        'class': 'md-input'
                    });

                    editor.append(textarea);

                    // Save the editable
                    editable.el = container;
                    editable.type = container.prop('tagName').toLowerCase();
                    editable.content = container.html();

                    $(container[0].attributes).each(function () {
                        editable.attrKeys.push(this.nodeName);
                        editable.attrValues.push(this.nodeValue);
                    });

                    // Set editor to blocked the original container
                    container.replaceWith(editor);
                }
                var rawText = textarea.val().trim(),
                    rawContent = rawText.substr(0, 1) != '<' && typeof toMarkdown == 'function' ? toMarkdown(rawText) : rawText;
                // inner content (whether textarea or others) is preferred
                var currentContent = $.trim(rawContent) || _localCache;
                textarea.val(currentContent);

                //add by wpl
                if (options.fullscreen.enable && _fullPreview === null) {
                    _fullPreview = $('<div/>', {
                        'class': 'md-full-preview'
                    });
                    var previewBody = $('<div/>', {
                        'class': 'md-full-preview-body'
                    });

                    innerPreview = $('<div/>', {
                        'class': 'md-full-preview-inner'
                    });
                    previewBody.append(innerPreview);
                    _fullPreview.append(previewBody);
                    var leftTool = $('<div/>', {
                        'class': 'md-full-preview-tool'
                    });
                    _fullPreview.append(leftTool);
                    editor.append(_fullPreview);

                    this.$innerPreview = innerPreview;
                    this.$fullPreview = _fullPreview;
                }

                var editorFooter = $('<div/>', {
                    'class': 'md-footer'
                }),
                    createFooter = false,
                    footer = '';
                // Create the footer if savable
                if (options.savable) {
                    createFooter = true;
                    var saveHandler = 'cmdSave';

                    // Register handler and callback
                    handler.push(saveHandler);
                    callback.push(options.onSave);

                    editorFooter.append('<button class="btn btn-success" data-provider="' + ns + '" data-handler="' + saveHandler + '"><i class="icon icon-white icon-ok"></i> ' + this.__localize('Save') + '</button>');
                }

                if (null === cutPaste) {
                    cutPaste = $('<div/>', {
                        class: 'md-cut-paste',
                        contenteditable: true
                    });

                    editor.append(cutPaste);
                }

                footer = typeof options.footer === 'function' ? options.footer(this) : options.footer;

                if ($.trim(footer) !== '') {
                    createFooter = true;
                    editorFooter.append(footer);
                }

                if (createFooter) editor.append(editorFooter);

                // Set width
                if (options.width && options.width !== 'inherit') {
                    if (jQuery.isNumeric(options.width)) {
                        editor.css('display', 'table');
                        textarea.css('width', options.width + 'px');
                    } else {
                        editor.addClass(options.width);
                    }
                }

                // Set height
                if (options.height && options.height !== 'inherit') {
                    if (jQuery.isNumeric(options.height)) {
                        var height = options.height;
                        if (editorHeader) height = Math.max(0, height - editorHeader.outerHeight());
                        if (editorFooter) height = Math.max(0, height - editorFooter.outerHeight());
                        textarea.css('height', height + 'px');
                    } else {
                        editor.addClass(options.height);
                    }
                }

                // Reference
                this.$editor = editor;
                this.$textarea = textarea;
                this.$editable = editable;
                this.$cutPaste = cutPaste;
                this.$oldContent = this.getContent();

                this.__setListener();

                // Set editor attributes, data short-hand API and listener
                this.$editor.attr('id', new Date().getTime().toString());

                this.$editor.on('click', '[data-provider="bootstrap-markdown"]', $.proxy(this.__handle, this));

                if (this.$element.is(':disabled') || this.$element.is('[readonly]')) {
                    this.$editor.addClass('md-editor-disabled');
                    this.disableButtons('all');
                }

                if (this.eventSupported('keydown') && _typeof(jQuery.hotkeys) === 'object') {
                    editorHeader.find('[data-provider="bootstrap-markdown"]').each(function () {
                        var $button = $(this),
                            hotkey = $button.attr('data-hotkey');
                        if (hotkey.toLowerCase() !== '') {
                            textarea.bind('keydown', hotkey, function () {
                                $button.trigger('click');
                                return false;
                            });
                        }
                    });
                }

                if (options.initialstate === 'preview') {
                    this.showPreview();
                } else if (options.initialstate === 'fullscreen' && options.fullscreen.enable) {
                    this.setFullscreen(true);
                }
            } else {
                this.$editor.show();
            }

            if (options.autofocus) {
                this.$textarea.focus();
                this.$editor.addClass('active');
            }

            if (options.fullscreen.enable && options.fullscreen !== false && !this.$fullscreenControls) {
                this.$editor.append('\
          <div class="md-fullscreen-controls">\
            <a href="#" class="exit-fullscreen" title="Exit fullscreen"><span class="' + this.__getIcon(options.fullscreen.icons.fullscreenOff) + '"></span></a>\
          </div>');
                this.$fullscreenControls = true;
                this.$editor.on('click', '.exit-fullscreen', function (e) {
                    e.preventDefault();
                    instance.setFullscreen(false);
                });
            }

            // hide hidden buttons from options
            this.hideButtons(options.hiddenButtons);

            // disable disabled buttons from options
            this.disableButtons(options.disabledButtons);

            // Trigger the onShow hook
            options.onShow(this);

            if (!this.$registPaste) {
                this.registPaste();
                this.$registPaste = true;
            }
            this.localCache();

            this.$loading = $('<i>', {
                class: 'md-loading fa fa-spinner fa-pulse'
            }).appendTo(this.$editor);

            this.bindDropUpload();

            // marked parse
            if (typeof marked === 'function') {
                //处理流程图和序列图
                var markedRenderer = new marked.Renderer();
                if (this.$options.flowChart) {
                    markedRenderer.code = function (code, lang) {
                        var diagramContent = null;
                        instance.$diagramCache.forEach(function (diagram) {
                            if (diagram.langTest.test(lang)) {
                                if (diagram.cache[code]) {
                                    diagramContent = diagram.cache[code];
                                } else {
                                    diagramContent = '<div class="' + diagram.className + ' diagram-raw">' + code + '</div>';
                                }
                            }
                        });
                        return diagramContent || marked.Renderer.prototype.code.apply(this, arguments);
                    };
                }
                this.markedParse = function (val) {
                    return marked(val, { renderer: markedRenderer });
                };
            }

            // diagram cache
            this.$diagramCache = [{
                langTest: /^flow|flowchart$/i,
                className: 'flowchart',
                jqueryFn: 'flowChart',
                cache: {}
            }, {
                langTest: /^seq|sequence$/i,
                className: 'sequence-diagram',
                jqueryFn: 'sequenceDiagram',
                opts: {
                    theme: 'simple'
                },
                cache: {}
            }];

            return this;
        },

        parseContent: function parseContent(val) {
            var content,
                _this = this;

            // parse with supported markdown parser
            var val = val || this.$textarea.val();
            if ((typeof markdown === 'undefined' ? 'undefined' : _typeof(markdown)) === 'object' && typeof markdown.toHTML === 'function') {
                content = markdown.toHTML(val);
            } else if (typeof this.markedParse === 'function') {
                content = this.markedParse(val);
            } else {
                content = val;
            }

            return content;
        },
        showUpload: function showUpload(e) {
            var _this = this,
                uploadPanel = this.$uploadPanel,
                editor = this.$editor,

            //upload panel
            mdUpload = null,
                mdDialog = null,
                mdContent = null,
                mdContentHeader = null,
                mdContentBody = null,
                mdContentFooter = null,
                inputGroup = null,
                localUpload = null,
                localUploadField = null,
                urlInput = null,
                stateBar = null,
                cancleButton = null,
                okButton = null,
                progressBar = null,
                progress = null,
                percent = null;
            if (this.$editor !== null && uploadPanel == null) {
                mdUpload = $('<div />', {
                    'class': 'md-upload',
                    'data-provide': 'markdown-upload'
                }).on('click', function (evt) {
                    if ($(evt.target).is('div.md-upload')) _this.hideUpload();
                });

                mdDialog = $('<div/>', {
                    'class': 'md-dialog',
                    'data-provide': 'markdown-upload-dialog'
                });

                mdContent = $('<div/>', {
                    'class': 'md-content',
                    'data-provide': 'markdown-upload-content'
                });

                mdContentHeader = $('<div/>', {
                    'class': 'md-content-header',
                    'data-provide': 'markdown-upload-content-header'
                }).append($('<i/>', {
                    type: 'button',
                    class: 'md-content-header-button gly gly-remove'
                })).on('click', function (evt) {
                    if ($(evt.target).is('i.md-content-header-button')) _this.hideUpload();
                }).append($('<h4/>', {
                    class: 'md-content-header-title title',
                    text: e.__localize('Image')
                }));

                mdContentBody = $('<div/>', {
                    'class': 'md-content-body',
                    'data-provide': 'markdown-upload-content-body'
                }).append($('<div/>', {
                    class: 'md-content-body-danger',
                    text: e.__localize('ImageTip')
                })).append($('<p/>', {
                    text: e.__localize('ImageInputTip')
                }));

                inputGroup = $('<div/>', {
                    class: 'md-content-body-input-group'
                });

                localUpload = $('<span />', {
                    class: 'md-input-group-addon gly gly-picture'
                });
                localUploadField = $('<input>', {
                    type: 'file',
                    multiple: 'multiple',
                    class: 'md-input-insert-image',
                    formenctype: 'multipart/form-data'
                });
                localUploadField.change(function () {
                    _this.fileUpload();
                });

                localUpload.on('click', function (evt) {
                    if (typeof FormData === "undefined") {
                        stateBar.html(e.__localize('BrowerSupportTip'));
                        return;
                    }
                    localUploadField.trigger('click');
                    return false;
                });

                urlInput = $('<textarea>', {
                    // type: 'text',
                    class: 'md-input-image-url',
                    placeholder: 'http://example.com/image.jpg'
                });

                progressBar = $('<div/>', { class: 'md-progress-bar' });
                progress = $('<progress/>', { max: 100, value: 0 });
                percent = $('<span/>', {
                    text: _this.__localize('Progress') + '0%'
                });

                progressBar.append(percent).append(progress);

                inputGroup.append(localUpload).append(localUploadField).append(urlInput);

                mdContentBody.append(inputGroup).append(progressBar);

                mdContentFooter = $('<div/>', {
                    'class': 'md-content-footer',
                    'data-provide': 'markdown-upload-content-footer'
                });

                stateBar = $('<span/>', { class: 'md-state-bar' });

                cancleButton = $('<button/>', {
                    class: 'btn btn-default',
                    type: 'button',
                    text: e.__localize('Cancle')
                });

                cancleButton.bind('click', function () {
                    _this.hideUpload();
                });

                okButton = $('<button/>', {
                    class: 'btn btn-primary',
                    text: e.__localize('Insert')
                });

                okButton.bind('click', function () {
                    var link = urlInput.val();
                    if (null === link || '' === link) {
                        _this.setState(_this.__localize('ImageInputTip'));
                        return false;
                    }
                    _this.setImageLink(link.split('\n'));
                    _this.setPercent(0);
                    return false;
                });

                mdContentFooter.append(stateBar).append(cancleButton).append(okButton);

                mdContent.append(mdContentHeader).append(mdContentBody).append(mdContentFooter);

                mdDialog.append(mdContent);

                editor.append(mdUpload.append(mdDialog));

                this.$uploadPanel = mdUpload;
                this.$inputFile = localUploadField;
                this.$progress = progress;
                this.$percent = percent;
                this.$stateBar = stateBar;
                return;
            }

            uploadPanel.show();
        },
        setPercent: function setPercent(progress) {
            if (this.$percent) {
                this.$percent.html(this.__localize('Progress') + progress + '%');
            }
        },
        setState: function setState(text, color) {
            var _this = this;
            if (_this.$stateBar) {
                if (color) {
                    _this.$stateBar.addClass('md-green');
                }
                _this.$stateBar.html(text);
                setTimeout(function () {
                    _this.$stateBar.html('');
                    _this.$stateBar.removeClass('md-green');
                }, 5000);
            }
        },
        fileUpload: function fileUpload() {
            //ajax上传文件
            var _this = this,
                imgUrl = this.$options.imgurl,
                xhr = null,
                progress = this.$progress,
                maxFileSize = this.$fileSize,
                uploadImgURL = "",
                uploadPanel = this.$uploadPanel,
                inputFile = this.$inputFile,
                _fileSize = 0,
                _fileName = '',
                _suffixReg = /^.*\.(?:jpg|png|gif)$/,
                formData = new FormData();
            if (progress && progress.length > 0) {
                progress = progress.get(0);
            }
            if (null === imgUrl || '' === imgUrl) {
                _this.setState(_this.__localize('UploadPathTip'));
                return;
            }

            if (inputFile.length > 0 && inputFile[0].files && inputFile[0].files.length > 0) {
                var _typeTip = [],
                    _sizeTip = [],
                    state = '';
                Array.prototype.forEach.call(inputFile[0].files, function (file) {
                    _fileSize = file.size;
                    _fileName = file.name.toLowerCase();

                    if (!_fileName.match(_suffixReg)) {
                        _typeTip.push(_fileName);
                    } else if (_fileSize > maxFileSize) {
                        _sizeTip.push(_fileName);
                    } else {
                        formData.append('img', file);
                    }
                });
                if (_typeTip.length) {
                    state += _this.__localize('SupportTypeTip') + ': ' + _typeTip.join(', ') + '. ';
                }
                if (_sizeTip.length) {
                    state += _this.__localize('FileSizeTip') + ': ' + _sizeTip.join(', ') + '. ';
                }
                if (state.length) {
                    _this.setState(state);
                }

                if (formData.has('img')) {
                    xhr = new XMLHttpRequest();
                    xhr.upload.onprogress = function (evt) {
                        _this.setPercent(Math.round(evt.loaded * 100 / evt.total));
                        progress.max = evt.total;
                        progress.value = evt.loaded;
                    };

                    xhr.upload.onload = function () {
                        setTimeout(function () {
                            _this.setPercent(0);
                            progress.max = 100;
                            progress.value = 0;
                            _this.setState(_this.__localize('ProgressLoaded'), true);
                        }, 1000);
                    };

                    xhr.upload.onerror = function () {
                        _this.setPercent(0);
                        progress.max = 100;
                        progress.value = 0;

                        uploadPanel.find('input.md-input-insert-image').val('');
                        uploadPanel.find('input.md-input-image-url').val('');

                        _this.setState(_this.__localize('UploadEooroTip'));
                    };

                    xhr.onreadystatechange = function () {
                        if (xhr.readyState === 4 && xhr.status === 200) {
                            uploadImgURL = xhr.responseText;
                            if ('' !== uploadImgURL) {
                                uploadPanel.find('textarea.md-input-image-url').val(uploadImgURL);
                            }
                        }
                    };

                    xhr.open('POST', imgUrl, true);
                    xhr.setRequestHeader("Cache-Control", "no-cache");
                    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
                    xhr.send(formData);
                }
            }
        },
        xhrImageUpload: function xhrImageUpload(base64) {
            var _this = this,
                base64Url = this.$options.base64url;
            if (null === base64Url || '' === base64Url) return;
            if (base64.indexOf("data:image/png;base64") !== -1) {
                var imageFormData = new FormData();
                imageFormData.append("base64Date", base64); // typo
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function () {
                    if (xhr.readyState === 4 && xhr.status === 200) {
                        var link = xhr.responseText;
                        if ('' !== link) {
                            _this.setImageLink(link);
                        }
                    }
                };
                xhr.upload.onerror = function () {
                    alert(_this.__localize('ImagePasteField'));
                };
                xhr.open("POST", base64Url, true);
                xhr.setRequestHeader("Cache-Control", "no-cache");
                xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
                xhr.send(imageFormData);
            }
        },
        bindDropUpload: function bindDropUpload() {
            var xhr;
            if (typeof 'XMLHttpRequest' !== 'undefined' && 'upload' in (xhr = new XMLHttpRequest())) {
                // IE 10+
                var _this = this,
                    imgUrl = _this.$options.imgurl,
                    maxFileSize = _this.$fileSize,
                    _suffixReg = /^.*\.(?:jpg|png|gif)$/i,
                    formData = new FormData(),
                    loadingCount = 0,
                    reader = new FileReader();
                reader.onload = function (e) {
                    var contents = e.target.result;
                    _this.replaceSelection(contents);
                };

                _this.$textarea.bind('dragover dragend', function () {
                    return false;
                }).bind("drop", function (e) {
                    var fileList = e.originalEvent.dataTransfer.files;
                    formData.delete('img');
                    if (fileList.length) {
                        // file drop
                        Array.prototype.forEach.call(fileList, function (file) {
                            if (file.type.indexOf('text') != -1) {
                                if (file.size > _this.$fileSize) {
                                    alert(_this.__localize('FileSizeTip'));
                                    return;
                                }
                                reader.readAsText(file);
                            } else {
                                if (null === imgUrl || '' === imgUrl) {
                                    alert(_this.__localize('UploadPathTip'));
                                    return;
                                }

                                if (!file.name.match(_suffixReg)) {
                                    alert(_this.__localize('SupportTypeTip') + ': ' + file.name);
                                    return;
                                }

                                if (file.size > maxFileSize) {
                                    alert(_this.__localize('FileSizeTip') + ': ' + file.name);
                                    return;
                                }
                                formData.append('img', file);
                            }
                        });

                        if (formData.has('img')) {
                            xhr.upload.onerror = function () {
                                _this.hideLoading(--loadingCount);
                                alert(_this.__localize('UploadEooroTip'));
                            };

                            xhr.onreadystatechange = function () {
                                if (xhr.readyState === 4 && xhr.status === 200) {
                                    _this.hideLoading(--loadingCount);
                                    _this.setImageLink(xhr.responseText.split('\n'));
                                }
                            };

                            xhr.open('POST', imgUrl, true);
                            xhr.setRequestHeader("Cache-Control", "no-cache");
                            xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
                            _this.hideLoading(++loadingCount);
                            xhr.send(formData);
                        }

                        return false;
                    }
                });
            }
        },
        setImageLink: function setImageLink(link) {
            // Give ![] surround the selection and prepend the image link
            var _this = this,
                chunk,
                cursor,
                instance = this,
                selected = instance.getSelection(),
                _links = Array.isArray(link) ? link : [link];

            if (selected.length === 0) {
                // Give extra word
                chunk = instance.__localize('enter image description here');
            } else {
                chunk = selected.text;
            }

            var linkContents = _links.filter(function (_link) {
                return _link !== null && _link !== '' && _link !== 'http://' && (_link.substr(0, 4) === 'http' || _link.substr(0, 21) === 'data:image/png;base64');
            }).map(function (_link) {
                var sanitizedLink = $('<div>' + _link + '</div>').text();

                // transform selection and set the cursor into chunked text
                return '![' + chunk + '](' + sanitizedLink + ' "' + instance.__localize('enter image title here') + '")';
            }).join('\n');
            if (linkContents.length) {
                instance.replaceSelection(linkContents);
                // Set the cursor and then set selection
                cursor = selected.start + 2;
                instance.setSelection(cursor, cursor + chunk.length);

                if (_this.$isFullscreen) {
                    _this.$innerPreview.html(_this.parseContent(_this.$textarea.val()));
                    _this.showDiagrams(_this.$innerPreview);
                }
            }
        },
        hideUpload: function hideUpload() {
            var uploadPanel = this.$uploadPanel,
                textarea = this.$textarea;
            if (null !== uploadPanel) {
                textarea.focus();
                uploadPanel.find('input.md-input-insert-image').val('');
                uploadPanel.find('input.md-input-image-url').val('');
                uploadPanel.hide();
                this.$uploadMode = false;
            }
        },
        hideLoading: function hideLoading(loadingCount) {
            this.$loading.toggle(loadingCount > 0);
        },
        localCache: function localCache() {
            var _localStorage = this.$localStorage,
                textarea = this.$textarea;
            if (window.localStorage && _localStorage && '' !== _localStorage) {
                setInterval(function () {
                    localStorage.setItem(_localStorage, textarea.val());
                }, 1000);
            }
        },
        registPaste: function registPaste() {
            var _this = this,
                cutPaste = this.$cutPaste,
                editor = this.$editor,
                timeStamp = null,
                browser = navigator.userAgent.toLowerCase();
            if (null === cutPaste) return;

            var firefox = false,
                chrome = true,
                trident = false;

            if (/firefox/i.test(browser)) {
                editor.keypress(function (event) {
                    _this.pasteFunc(event, firefox);
                });
                //这里处理Firefox粘贴处理
                editor.get(0).addEventListener('paste', function (event) {
                    var event = event || window.event;
                    var clipboardData = event.clipboardData || window.clipboardData;
                    var text = clipboardData.getData("text");
                    if (text) {
                        var selection = _this.getSelection().start;
                        _this.replaceSelection(text);
                        _this.setSelection(selection + text.length, selection + text.length);
                        if (event.preventDefault) {
                            event.preventDefault();
                            event.stopPropagation();
                        }
                        if (event.returnValue) event.returnValue = false;
                        if (event.cancelBubble) event.cancelBubble = true;
                        return false;
                    }
                }, true);
            } else if (/chrome/i.test(browser) && /webkit/i.test(browser) && /mozilla/i.test(browser)) {
                chrome = true;
                editor.on('paste', function (event) {
                    _this.pasteFunc(event, chrome);
                });
            } else if (/trident/i.test(browser)) {
                editor.keydown(function (event) {
                    _this.pasteFunc(event, trident);
                });
            }
        },
        pasteFunc: function pasteFunc(event, chrome) {
            var _this = this,
                cutPaste = this.$cutPaste,
                textarea = this.$textarea,
                uploadMode = this.$uploadMode;

            if (!chrome) {
                //防止一个粘贴BUG
                if (uploadMode) return;
            }
            if (!chrome && (event.ctrlKey || event.metaKey) && (event.keyCode === 86 || event.key === 'v')) {
                cutPaste.focus();
                setTimeout(function () {
                    var imgs = cutPaste.find('img'),
                        img = null,
                        base64 = null;
                    textarea.focus();
                    if (imgs && imgs.length > 0) {
                        img = imgs[0];
                        base64 = img.src;
                        if (base64 && '' !== base64) {
                            _this.xhrImageUpload(base64);
                        }
                        imgs.remove();
                    }
                    var text = '';
                    //这里不作Firefox粘贴处理
                    if (window.clipboardData) {
                        text = window.clipboardData.getData('Text');
                        if (text) {
                            var selection = _this.getSelection().start;
                            _this.replaceSelection(text);
                            _this.setSelection(selection + text.length, selection + text.length);
                        }
                    }
                    cutPaste.empty();
                }, 10);
            } else if (chrome) {
                var clipboardData,
                    items,
                    item,
                    _i = 0,
                    _length,
                    _ref;
                if (((_ref = event.originalEvent) !== null ? _ref.clipboardData : void 0) !== null) {
                    clipboardData = event.originalEvent.clipboardData;
                    if (items = clipboardData.items) {
                        _length = items.length;
                        for (; _i < _length; ++_i) {
                            item = items[_i];
                            if (item && item.type.match(/^image\//)) {
                                var blob = item.getAsFile(),
                                    reader = new FileReader(),
                                    base64 = null;
                                reader.onload = function (evt) {
                                    base64 = evt.target.result;
                                    if (base64 && '' !== base64) {
                                        _this.xhrImageUpload(base64);
                                    }
                                };
                                reader.readAsDataURL(blob);
                            }
                        }
                    }
                }
            }
        },
        showDiagrams: function showDiagrams($container) {
            // flow chart, sequence diagram, math expression
            if (!this.isIE8) {
                if (this.$options.flowChart) {
                    this.$diagramCache.forEach(function (diagram) {
                        $container.find('.' + diagram.className + '.diagram-raw').each(function () {
                            var $el = $(this),
                                code = $el.text(),
                                content = $el[diagram.jqueryFn](diagram.opts).removeClass('diagram-raw').get(0).outerHTML;
                            diagram.cache[code] = content;
                        });
                    });
                }
            }
        },
        showPreview: function showPreview() {
            var options = this.$options,
                container = this.$textarea,
                afterContainer = container.next(),
                replacementContainer = $('<div/>', { 'class': 'md-preview', 'data-provider': 'markdown-preview' }),
                content,
                callbackContent;

            // Give flag that tell the editor enter preview mode
            this.$isPreview = true;
            // Disable all buttons
            this.disableButtons('all').enableButtons('cmdPreview');

            // Try to get the content from callback
            callbackContent = options.onPreview(this);
            // Set the content based from the callback content if string otherwise parse value from textarea
            content = typeof callbackContent == 'string' ? callbackContent : this.parseContent();

            // Build preview element
            replacementContainer.html(content);

            if (afterContainer && afterContainer.attr('class') == 'md-footer') {
                // If there is footer element, insert the preview container before it
                replacementContainer.insertBefore(afterContainer);
            } else {
                // Otherwise, just append it after textarea
                container.parent().append(replacementContainer);
            }
            this.showDiagrams(replacementContainer);

            // Set the preview element dimensions
            replacementContainer.css({
                //width: container.outerWidth() + 'px',
                width: 'inherit',
                height: container.outerHeight() + 'px'
            });

            if (this.$options.resize) {
                replacementContainer.css('resize', this.$options.resize);
            }

            // Hide the last-active textarea
            container.hide();

            // Attach the editor instances
            replacementContainer.data('markdown', this);

            if (this.$element.is(':disabled') || this.$element.is('[readonly]')) {
                this.$editor.addClass('md-editor-disabled');
                this.disableButtons('all');
            }
            return this;
        },

        hidePreview: function hidePreview() {
            // Give flag that tell the editor quit preview mode
            this.$isPreview = false;

            // Obtain the preview container
            var container = this.$editor.find('div[data-provider="markdown-preview"]');

            //

            // Remove the preview container
            container.remove();

            // Enable all buttons
            this.enableButtons('all');
            // Disable configured disabled buttons
            this.disableButtons(this.$options.disabledButtons);

            // Back to the editor
            this.$textarea.show();

            return this;
        },

        isDirty: function isDirty() {
            return this.$oldContent != this.getContent();
        },

        getContent: function getContent() {
            return this.$textarea.val();
        },

        setContent: function setContent(content) {
            this.$textarea.val(content);

            return this;
        },

        findSelection: function findSelection(chunk) {
            var content = this.getContent(),
                startChunkPosition;

            if (startChunkPosition = content.indexOf(chunk), startChunkPosition >= 0 && chunk.length > 0) {
                var oldSelection = this.getSelection(),
                    selection;

                this.setSelection(startChunkPosition, startChunkPosition + chunk.length);
                selection = this.getSelection();

                this.setSelection(oldSelection.start, oldSelection.end);

                return selection;
            } else {
                return null;
            }
        },
        getSelection: function getSelection() {

            var e = this.$textarea[0];

            return ('selectionStart' in e && function () {
                var l = e.selectionEnd - e.selectionStart;
                return {
                    start: e.selectionStart,
                    end: e.selectionEnd,
                    length: l,
                    text: e.value.substr(e.selectionStart, l)
                };
            } ||

            /* browser not supported */
            function () {
                return null;
            })();
        },

        setSelection: function setSelection(start, end) {

            var e = this.$textarea[0];

            return ('selectionStart' in e && function () {
                e.selectionStart = start;
                e.selectionEnd = end;
                return;
            } ||

            /* browser not supported */
            function () {
                return null;
            })();
        },

        replaceSelection: function replaceSelection(text) {

            var e = this.$textarea[0];

            return ('selectionStart' in e && function () {
                var start = e.selectionStart + text.length;
                e.value = e.value.substr(0, e.selectionStart) + text + e.value.substr(e.selectionEnd, e.value.length);

                // Set cursor to the last replacement end
                e.selectionStart = e.selectionEnd = start;
                return this;
            } ||
            /* browser not supported */
            function () {
                e.value += text;
                return jQuery(e);
            })();
        },

        getNextTab: function getNextTab() {
            // Shift the nextTab
            if (this.$nextTab.length === 0) {
                return null;
            } else {
                var nextTab,
                    tab = this.$nextTab.shift();

                if (typeof tab == 'function') {
                    nextTab = tab();
                } else if ((typeof tab === 'undefined' ? 'undefined' : _typeof(tab)) == 'object' && tab.length > 0) {
                    nextTab = tab;
                }

                return nextTab;
            }
        },

        setNextTab: function setNextTab(start, end) {
            // Push new selection into nextTab collections
            if (typeof start == 'string') {
                var that = this;
                this.$nextTab.push(function () {
                    return that.findSelection(start);
                });
            } else if (typeof start == 'number' && typeof end == 'number') {
                var oldSelection = this.getSelection();

                this.setSelection(start, end);
                this.$nextTab.push(this.getSelection());

                this.setSelection(oldSelection.start, oldSelection.end);
            }

            return;
        },

        __parseButtonNameParam: function __parseButtonNameParam(nameParam) {
            var buttons = [];

            if (typeof nameParam == 'string') {
                buttons = nameParam.split(',');
            } else {
                buttons = nameParam;
            }

            return buttons;
        },

        enableButtons: function enableButtons(name) {
            var buttons = this.__parseButtonNameParam(name),
                that = this;

            $.each(buttons, function (i, v) {
                that.__alterButtons(buttons[i], function (el) {
                    el.removeAttr('disabled');
                });
            });

            return this;
        },

        disableButtons: function disableButtons(name) {
            var buttons = this.__parseButtonNameParam(name),
                that = this;

            $.each(buttons, function (i, v) {
                that.__alterButtons(buttons[i], function (el) {
                    el.attr('disabled', 'disabled');
                });
            });

            return this;
        },

        hideButtons: function hideButtons(name) {
            var buttons = this.__parseButtonNameParam(name),
                that = this;

            $.each(buttons, function (i, v) {
                that.__alterButtons(buttons[i], function (el) {
                    el.addClass('hidden');
                });
            });

            return this;
        },

        showButtons: function showButtons(name) {
            var buttons = this.__parseButtonNameParam(name),
                that = this;

            $.each(buttons, function (i, v) {
                that.__alterButtons(buttons[i], function (el) {
                    el.removeClass('hidden');
                });
            });

            return this;
        },

        eventSupported: function eventSupported(eventName) {
            var isSupported = eventName in this.$element;
            if (!isSupported) {
                this.$element.setAttribute(eventName, 'return;');
                isSupported = typeof this.$element[eventName] === 'function';
            }
            return isSupported;
        },

        keydown: function keydown(e) {
            // use keydown event to preventDefault
            var blocked = false;
            switch (e.keyCode) {
                case 40: // down arrow
                case 38: // up arrow
                case 16: // shift
                case 17: // ctrl
                case 18:
                    // alt
                    break;

                case 9:
                    // tab
                    var nextTab;
                    if (nextTab = this.getNextTab(), nextTab !== null) {
                        // Get the nextTab if exists
                        var that = this;
                        setTimeout(function () {
                            that.setSelection(nextTab.start, nextTab.end);
                        }, 500);

                        blocked = true;
                    } else {// just output tabs
                        /*// The next tab memory contains nothing...
                        // check the cursor position to determine tab action
                        var cursor = this.getSelection();
                         if (cursor.start == cursor.end && cursor.end == this.getContent().length) {
                            // The cursor already reach the end of the content
                            blocked = false;
                        } else {
                            // Put the cursor to the end
                            this.setSelection(this.getContent().length, this.getContent().length);
                             blocked = true;
                        }*/
                    }

                    break;

                case 13:
                    // enter
                    blocked = false;
                    break;
                case 27:
                    // escape
                    if (this.$isFullscreen) this.setFullscreen(false);
                    blocked = false;
                    break;

                default:
                    blocked = false;
            }

            if (blocked) {
                e.stopPropagation();
                e.preventDefault();
            }
        },

        keyup: function keyup(e) {
            this.$options.onChange(this);
        },

        change: function change(e) {
            this.$options.onChange(this);
            return this;
        },

        focus: function focus(e) {
            var options = this.$options,
                isHideable = options.hideable,
                editor = this.$editor;

            editor.addClass('active');

            // Blur other markdown(s)
            $(document).find('.md-editor').each(function () {
                if ($(this).attr('id') !== editor.attr('id')) {
                    var attachedMarkdown;

                    if (attachedMarkdown = $(this).find('textarea').data('markdown'), attachedMarkdown === null) {
                        attachedMarkdown = $(this).find('div[data-provider="markdown-preview"]').data('markdown');
                    }

                    if (attachedMarkdown) {
                        attachedMarkdown.blur();
                    }
                }
            });

            // Trigger the onFocus hook
            options.onFocus(this);

            return this;
        },

        blur: function blur(e) {
            var options = this.$options,
                isHideable = options.hideable,
                editor = this.$editor,
                editable = this.$editable;

            if (editor.hasClass('active') || this.$element.parent().length === 0) {
                editor.removeClass('active');

                if (isHideable) {
                    // Check for editable elements
                    if (editable.el !== null) {
                        // Build the original element
                        var oldElement = $('<' + editable.type + '/>'),
                            content = this.getContent(),
                            currentContent = (typeof markdown === 'undefined' ? 'undefined' : _typeof(markdown)) == 'object' && typeof markdown.toHTML === 'function' ? markdown.toHTML(content) : content;

                        $(editable.attrKeys).each(function (k, v) {
                            oldElement.attr(editable.attrKeys[k], editable.attrValues[k]);
                        });

                        // Get the editor content
                        oldElement.html(currentContent);

                        editor.replaceWith(oldElement);
                    } else {
                        editor.hide();
                    }
                }

                // Trigger the onBlur hook
                options.onBlur(this);
            }

            return this;
        },
        showEmojiPanel: function showEmojiPanel(e) {
            var _this = this,
                emojiPanel = this.$emojiPanel,
                emojiElements = this.$emojiElements,
                editor = this.$editor,

            //emojiPanel panel
            mdEmoji = null,
                mdDialog = null,
                mdContent = null,
                mdContentHeader = null,
                mdContentBody = null,
                mdContentFooter = null,
                textarea = this.$textarea,
                fullScreen = this.$isFullscreen,
                innerPreview = this.$innerPreview;

            if (emojiElements === null) {
                emojiElements = _this.initEmoji();
            }

            if (this.$editor !== null && emojiPanel == null) {
                mdEmoji = $('<div />', {
                    'class': 'md-emoji',
                    'data-provide': 'markdown-emoji'
                }).on('click', function (evt) {
                    if ($(evt.target).is('div.md-emoji')) _this.hideEmoji();
                });

                mdDialog = $('<div/>', {
                    'class': 'md-dialog',
                    'data-provide': 'markdown-emoji-dialog'
                });

                mdContent = $('<div/>', {
                    'class': 'md-content',
                    'data-provide': 'markdown-upload-content'
                });

                mdContentHeader = $('<div/>', {
                    'class': 'md-content-header',
                    'data-provide': 'markdown-upload-content-header'
                }).append($('<i/>', {
                    type: 'button',
                    class: 'md-content-header-button gly gly-remove'
                })).on('click', function (evt) {
                    if ($(evt.target).is('i.md-content-header-button')) _this.hideEmoji();
                }).append($('<Strong/>', {
                    class: 'md-content-header-title',
                    text: e.__localize('Emoji')
                }));

                mdContentBody = $('<div/>', {
                    'class': 'md-content-body md-emoji',
                    'data-provide': 'markdown-upload-content-body'
                }).append(emojiElements);

                mdContent.append(mdContentHeader).append(mdContentBody).append(mdContentFooter);
                mdDialog.append(mdContent);
                editor.append(mdEmoji.append(mdDialog));

                emojiElements.on('click', function (evt) {
                    var __this = $(this),
                        groupName = _this.$emoji['groupName'],
                        groupNav = _this.$emoji['groupNav'],
                        groupPanel = _this.$emoji['groupPanel'],
                        _target = evt.target;
                    if (_target) {
                        var tagName = _target.tagName;
                        if (tagName === 'LI') {
                            var nav = _target.getAttribute('data-emoji-target');
                            for (var i = 0; i < groupName.length; ++i) {
                                var __groupName = groupName[i];
                                if (__groupName === nav) {
                                    groupNav[__groupName].addClass('active');
                                    groupPanel[__groupName].show().scrollTop(0);
                                    continue;
                                }
                                groupNav[__groupName].removeClass('active');
                                groupPanel[__groupName].hide();
                            }
                            return;
                        }

                        var emojiKeyword = _target.getAttribute('data-emoji');
                        if (tagName === 'DIV' && emojiKeyword) {
                            // Give/remove ** surround the selection
                            var selected = e.getSelection(),
                                keywordLength = emojiKeyword.length + 2;
                            e.replaceSelection(":" + emojiKeyword + ":");
                            e.setSelection(selected.start + keywordLength, selected.end + keywordLength);
                            e.hideEmoji();
                            if (fullScreen) {
                                innerPreview.html(_this.parseContent(textarea.val()));
                                _this.showDiagrams(innerPreview);
                            }
                        }
                    }
                });

                this.$emojiPanel = mdEmoji;
                this.$emojiElements = emojiElements;
                return;
            }
            emojiPanel.show();
        },
        hideEmoji: function hideEmoji() {
            var textarea = this.$textarea,
                emojiPanel = this.$emojiPanel;
            if (null != emojiPanel) {
                emojiPanel.hide();
            }
            textarea.focus();
        },
        initEmoji: function initEmoji() {

            var emoji = this.$emoji,
                emojiGroup = emoji['groupName'],
                emojiPanel = $('<div/>', {
                class: 'md-emoji-panel'
            });
            var emojiNavPanel = $('<ul/>', {
                class: 'md-emoji-nav'
            });
            for (var egIndex = 0; egIndex < emojiGroup.length; ++egIndex) {
                var group = emoji['groups'][emojiGroup[egIndex]],
                    nav = this.renderEmojiNav(emojiGroup[egIndex], !egIndex);
                this.$emoji['groupNav'][emojiGroup[egIndex]] = nav;
                emojiNavPanel.append(nav);
                if (group instanceof Array) {

                    /*var _panel = this.renderEmoji(group, emojiGroup[egIndex], false);*/
                    var _panel = $('<div/>', {
                        'data-group': emojiGroup[egIndex]
                    });
                    if (emojiGroup[egIndex] === 'twemoji') {
                        _panel.append(twemoji);
                    } else if (emojiGroup[egIndex] === 'font-awesome') {
                        _panel.append(fontAwesome);
                    }
                    this.$emoji['groupPanel'][emojiGroup[egIndex]] = _panel;
                    emojiPanel.append(_panel.hide());
                    continue;
                }
                if (group instanceof Object) {
                    var githubPanel = $('<div/>', {
                        'data-group': emojiGroup[egIndex]
                    }),
                        githubGroup = ['People', 'Nature', 'Objects', 'Places', 'Symbols'];
                    for (var ghIndex = 0; ghIndex < githubGroup.length; ++ghIndex) {
                        var name = githubGroup[ghIndex],
                            github = group[name];
                        if (github instanceof Array) {
                            //githubPanel.append(this.renderEmoji(github, name, true));
                            if (name === 'People') {
                                githubPanel.append(githubEmojiPeople);
                            } else if (name === 'Nature') {
                                githubPanel.append(githubEmojiNature);
                            } else if (name === 'Objects') {
                                githubPanel.append(githubEmojiObjects);
                            } else if (name === 'Places') {
                                githubPanel.append(githubEmojiPlaces);
                            } else if (name === 'Symbols') {
                                githubPanel.append(githubEmojiSymbols);
                            }
                        }
                    }
                    this.$emoji['groupPanel'][emojiGroup[egIndex]] = githubPanel;
                    emojiPanel.append(emojiNavPanel).append(githubPanel);
                }
            }

            return emojiPanel;
        },
        renderEmojiNav: function renderEmojiNav(name, active) {
            var _class = active ? 'active' : '';
            return $('<li/>', {
                class: _class,
                'data-emoji-target': name,
                text: name
            });
        },
        renderEmoji: function renderEmoji(group, name, title) {
            var cols = 20,
                fontawesome = ' fa ',
                groupPanel = $('<div/>', {
                'data-group': name
            }),
                groupLength = group.length,
                mod = groupLength % cols,
                rows = mod === 0 ? groupLength / cols : parseInt(groupLength / cols) + 1;
            if (title) {
                groupPanel.append($('<div/>', {
                    'data-group-title': 'emoji-group-title',
                    text: name
                }));
            }
            for (var row = 0; row < rows; row++) {
                var rowGroup = $('<div/>', {
                    class: 'emoji-row'
                });
                for (var col = 0; col < cols; col++) {
                    var index = row * cols + col,
                        emojiName = group[index],
                        _class = 'emoji-block';

                    var emojiHtml = $('<div/>', {
                        class: _class,
                        title: emojiName,
                        'data-emoji': emojiName
                    });

                    if (name === 'font-awesome') {
                        _class += fontawesome;
                        _class += emojiName;
                        emojiHtml.attr({
                            class: _class,
                            'data-class': fontawesome + emojiName
                        });
                    }
                    rowGroup.append(emojiHtml);
                }
                groupPanel.append(rowGroup);
            }

            return groupPanel;
        }

    };

    /* MARKDOWN PLUGIN DEFINITION
     * ========================== */

    var old = $.fn.markdown;

    $.fn.markdown = function (option) {
        return this.each(function () {
            var $this = $(this),
                data = $this.data('markdown'),
                options = (typeof option === 'undefined' ? 'undefined' : _typeof(option)) == 'object' && option;
            if (!data) $this.data('markdown', data = new Markdown(this, options));
        });
    };

    $.fn.markdown.messages = {};

    $.fn.markdown.defaults = {
        /* Editor Properties */
        autofocus: true, //是否自动焦点到编辑器
        hideable: false,
        savable: false,
        width: 'inherit',
        height: 'inherit',
        resize: 'none',
        iconlibrary: 'glyph',
        language: 'en', //默认国际化语言
        initialstate: 'editor',
        imgurl: '',
        base64url: '',
        localStorage: '',
        flowChart: false, //    flowChart.js 只支持 IE9+
        /* Buttons Properties */
        buttons: [[{
            name: 'groupFont',
            data: [{
                name: 'cmdBold',
                hotkey: 'Ctrl+B',
                title: 'Bold',
                icon: { glyph: 'gly gly-bold', fa: 'fa fa-bold', 'fa-3': 'icon-bold' },
                callback: function callback(e) {
                    // Give/remove ** surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('strong text');
                    } else {
                        chunk = selected.text;
                    }

                    // transform selection and set the cursor into chunked text
                    if (content.substr(selected.start - 2, 2) === '**' && content.substr(selected.end, 2) === '**') {
                        e.setSelection(selected.start - 2, selected.end + 2);
                        e.replaceSelection(chunk);
                        cursor = selected.start - 2;
                    } else {
                        e.replaceSelection('**' + chunk + '**');
                        cursor = selected.start + 2;
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }, {
                name: 'cmdItalic',
                title: 'Italic',
                hotkey: 'Ctrl+I',
                icon: { glyph: 'gly gly-italic', fa: 'fa fa-italic', 'fa-3': 'icon-italic' },
                callback: function callback(e) {
                    // Give/remove * surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('emphasized text');
                    } else {
                        chunk = selected.text;
                    }

                    // transform selection and set the cursor into chunked text
                    if (content.substr(selected.start - 1, 1) === '_' && content.substr(selected.end, 1) === '_') {
                        e.setSelection(selected.start - 1, selected.end + 1);
                        e.replaceSelection(chunk);
                        cursor = selected.start - 1;
                    } else {
                        e.replaceSelection('_' + chunk + '_');
                        cursor = selected.start + 1;
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }, {
                name: 'cmdHeading',
                title: 'Heading',
                hotkey: 'Ctrl+H',
                icon: { glyph: 'gly gly-header', fa: 'fa fa-header', 'fa-3': 'icon-font' },
                callback: function callback(e) {
                    // Append/remove ### surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent(),
                        pointer = 4,
                        prevChar;

                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('heading text');
                    } else {
                        chunk = selected.text + '\n';
                    }

                    // transform selection and set the cursor into chunked text
                    if (content.substr(selected.start - pointer, pointer) === '### ' || content.substr(selected.start - --pointer, pointer) === '###') {
                        e.setSelection(selected.start - pointer, selected.end);
                        e.replaceSelection(chunk);
                        cursor = selected.start - pointer;
                    } else if (selected.start > 0 && (prevChar = content.substr(selected.start - 1, 1), !!prevChar && prevChar != '\n')) {
                        e.replaceSelection('\n\n### ' + chunk);
                        cursor = selected.start + 6;
                    } else {
                        // Empty string before element
                        e.replaceSelection('### ' + chunk);
                        cursor = selected.start + 4;
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }]
        }, {
            name: 'groupLink',
            data: [{
                name: 'cmdUrl',
                title: 'URL/Link',
                hotkey: 'Ctrl+L',
                icon: { glyph: 'gly gly-link', fa: 'fa fa-link', 'fa-3': 'icon-link' },
                callback: function callback(e) {
                    // Give [] surround the selection and prepend the link
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent(),
                        link;

                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('enter link description here');
                    } else {
                        chunk = selected.text;
                    }

                    /* link = prompt(e.__localize('Insert Hyperlink'), 'http://');*/
                    link = 'http://';
                    /*  if (link !== null && link !== '' && link !== 'http://' && link.substr(0, 4) === 'http') {*/
                    var sanitizedLink = $('<div>' + link + '</div>').text();

                    // transform selection and set the cursor into chunked text
                    e.replaceSelection('[' + chunk + '](' + sanitizedLink + ')');
                    cursor = selected.start + chunk.length + 10;

                    // Set the cursor
                    e.setSelection(cursor, cursor);
                    /* }*/
                }
            }, {
                name: 'cmdImage',
                title: 'Image',
                hotkey: 'Ctrl+G',
                icon: { glyph: 'gly gly-picture', fa: 'fa fa-picture-o', 'fa-3': 'icon-picture' },
                callback: function callback(e) {
                    e.$uploadMode = true;
                    e.showUpload(e);
                }
            }, {
                name: 'cmdEmoji',
                title: 'Emoji',
                hotkey: 'Ctrl+E',
                icon: { glyph: 'fa fa-smile-o', fa: 'fa fa-smile-o', 'fa-3': 'icon-picture' },
                callback: function callback(e) {
                    //e.$uploadMode = true;
                    e.showEmojiPanel(e);
                }
            }]
        }, {
            name: 'groupMisc',
            data: [{
                name: 'cmdList',
                hotkey: 'Ctrl+U',
                title: 'Unordered List',
                icon: { glyph: 'gly gly-list', fa: 'fa fa-list', 'fa-3': 'icon-list-ul' },
                callback: function callback(e) {
                    // Prepend/Give - surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    // transform selection and set the cursor into chunked text
                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('list text here');

                        e.replaceSelection('- ' + chunk);
                        // Set the cursor
                        cursor = selected.start + 2;
                    } else {
                        if (selected.text.indexOf('\n') < 0) {
                            chunk = selected.text;

                            e.replaceSelection('- ' + chunk);

                            // Set the cursor
                            cursor = selected.start + 2;
                        } else {
                            var list = [];

                            list = selected.text.split('\n');
                            chunk = list[0];

                            $.each(list, function (k, v) {
                                list[k] = '- ' + v;
                            });

                            e.replaceSelection('\n\n' + list.join('\n'));

                            // Set the cursor
                            cursor = selected.start + 4;
                        }
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }, {
                name: 'cmdListO',
                hotkey: 'Ctrl+O',
                title: 'Ordered List',
                icon: { glyph: 'gly gly-th-list', fa: 'fa fa-list-ol', 'fa-3': 'icon-list-ol' },
                callback: function callback(e) {

                    // Prepend/Give - surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    // transform selection and set the cursor into chunked text
                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('list text here');
                        e.replaceSelection('1. ' + chunk);
                        // Set the cursor
                        cursor = selected.start + 3;
                    } else {
                        if (selected.text.indexOf('\n') < 0) {
                            chunk = selected.text;

                            e.replaceSelection('1. ' + chunk);

                            // Set the cursor
                            cursor = selected.start + 3;
                        } else {
                            var list = [];

                            list = selected.text.split('\n');
                            chunk = list[0];

                            $.each(list, function (k, v) {
                                list[k] = '1. ' + v;
                            });

                            e.replaceSelection('\n\n' + list.join('\n'));

                            // Set the cursor
                            cursor = selected.start + 5;
                        }
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }, {
                name: 'cmdCode',
                hotkey: 'Ctrl+K',
                title: 'Code',
                icon: { glyph: 'gly gly-asterisk', fa: 'fa fa-code', 'fa-3': 'icon-code' },
                callback: function callback(e) {
                    // Give/remove ** surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('code text here');
                    } else {
                        chunk = selected.text;
                    }

                    // transform selection and set the cursor into chunked text
                    if (content.substr(selected.start - 4, 4) === '```\n' && content.substr(selected.end, 4) === '\n```') {
                        e.setSelection(selected.start - 4, selected.end + 4);
                        e.replaceSelection(chunk);
                        cursor = selected.start - 4;
                    } else if (content.substr(selected.start - 1, 1) === '`' && content.substr(selected.end, 1) === '`') {
                        e.setSelection(selected.start - 1, selected.end + 1);
                        e.replaceSelection(chunk);
                        cursor = selected.start - 1;
                    } else if (content.indexOf('\n') > -1) {
                        e.replaceSelection('```\n' + chunk + '\n```');
                        cursor = selected.start + 4;
                    } else {
                        e.replaceSelection('`' + chunk + '`');
                        cursor = selected.start + 1;
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }, {
                name: 'cmdQuote',
                hotkey: 'Ctrl+Q',
                title: 'Quote',
                icon: { glyph: 'gly gly-comment', fa: 'fa fa-quote-left', 'fa-3': 'icon-quote-left' },
                callback: function callback(e) {
                    // Prepend/Give - surround the selection
                    var chunk,
                        cursor,
                        selected = e.getSelection(),
                        content = e.getContent();

                    // transform selection and set the cursor into chunked text
                    if (selected.length === 0) {
                        // Give extra word
                        chunk = e.__localize('quote here');

                        e.replaceSelection('> ' + chunk);

                        // Set the cursor
                        cursor = selected.start + 2;
                    } else {
                        if (selected.text.indexOf('\n') < 0) {
                            chunk = selected.text;

                            e.replaceSelection('> ' + chunk);

                            // Set the cursor
                            cursor = selected.start + 2;
                        } else {
                            var list = [];

                            list = selected.text.split('\n');
                            chunk = list[0];

                            $.each(list, function (k, v) {
                                list[k] = '> ' + v;
                            });

                            e.replaceSelection('\n\n' + list.join('\n'));

                            // Set the cursor
                            cursor = selected.start + 4;
                        }
                    }

                    // Set the cursor
                    e.setSelection(cursor, cursor + chunk.length);
                }
            }]
        }, {
            name: 'groupUtil',
            data: [{
                name: 'cmdPreview',
                toggle: true,
                hotkey: 'Ctrl+P',
                title: 'Preview',
                //btnText: 'Preview',
                //btnClass: 'btn btn-primary btn-sm',
                icon: { glyph: 'gly gly-search', fa: 'fa fa-search', 'fa-3': 'icon-search' },
                callback: function callback(e) {
                    // Check the preview mode and toggle based on this flag
                    var isPreview = e.$isPreview,
                        content;

                    if (isPreview === false) {
                        // Give flag that tell the editor enter preview mode
                        e.showPreview();
                    } else {
                        e.hidePreview();
                    }
                }
            }]
        }]],
        additionalButtons: [], // Place to hook more buttons by code
        reorderButtonGroups: [],
        hiddenButtons: [], // Default hidden buttons
        disabledButtons: [], // Default disabled buttons
        footer: '',
        fullscreen: {
            enable: true,
            icons: {
                fullscreenOn: {
                    fa: 'fa fa-expand',
                    glyph: 'gly gly-fullscreen',
                    'fa-3': 'icon-resize-full'
                },
                fullscreenOff: {
                    fa: 'fa fa-compress',
                    glyph: 'gly gly-fullscreen',
                    'fa-3': 'icon-resize-small'
                }
            },
            debounce: 0
        },

        /* Events hook */
        onShow: function onShow(e) {},
        onPreview: function onPreview(e) {},
        onSave: function onSave(e) {},
        onBlur: function onBlur(e) {},
        onFocus: function onFocus(e) {},
        onChange: function onChange(e) {},
        onFullscreen: function onFullscreen(e) {}
    };

    $.fn.markdown.Constructor = Markdown;

    /* MARKDOWN NO CONFLICT
     * ==================== */

    $.fn.markdown.noConflict = function () {
        $.fn.markdown = old;
        return this;
    };

    /* MARKDOWN GLOBAL FUNCTION & DATA-API
     * ==================================== */
    var initMarkdown = function initMarkdown(el) {
        var $this = el;

        if ($this.data('markdown')) {
            $this.data('markdown').showEditor();
            return;
        }
        $this.markdown();
    };

    var blurNonFocused = function blurNonFocused(e) {
        var $activeElement = $(document.activeElement);

        // Blur event
        $(document).find('.md-editor').each(function () {
            var $this = $(this),
                focused = $activeElement.closest('.md-editor')[0] === this,
                attachedMarkdown = $this.find('textarea').data('markdown') || $this.find('div[data-provider="markdown-preview"]').data('markdown');

            if (attachedMarkdown && !focused) {
                attachedMarkdown.blur();
            }
        });
    };
    $(document).on('click.markdown.data-api', '[data-provide="markdown-editable"]', function (e) {
        initMarkdown($(this));
        e.preventDefault();
    }).on('click focusin', function (e) {
        blurNonFocused(e);
    }).ready(function () {
        $('textarea[data-provide="markdown"]').each(function () {
            initMarkdown($(this));
        });
    });
}(window.jQuery);
//# sourceMappingURL=bootstrap-markdown.js.map
