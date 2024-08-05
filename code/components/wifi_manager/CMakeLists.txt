idf_component_register(SRC_DIRS src
        REQUIRES log nvs_flash mdns wpa_supplicant lwip esp_http_server esp_wifi
        INCLUDE_DIRS src
        EMBED_FILES src/assets/styles/style.css src/assets/scripts/code.js src/assets/index.html)