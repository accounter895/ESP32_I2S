#include "my_deepseek.h"

const char* deepseek_api_key = "b1c9428f-5f56-43e3-bd18-19050e53c221";

String apiUrl = "https://ark.cn-beijing.volces.com/api/v3/chat/completions";

String My_GPT::getGPTAnswer(String inputText) {
    HTTPClient http;
    http.setTimeout(65534);
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    String token_key = String("Bearer ") + deepseek_api_key;
    http.addHeader("Authorization", token_key);
    String payload = String("{\"model\":\"ep-20250216202252-wprvx\",")
                    + "\"messages\":[{\"role\":\"system\","
                    + "\"content\":\"你是个农业专家,接下来的农业问题你必须用中文回答,且字数限制在40字以内\"},"
                    + "{\"role\":\"user\","
                    + "\"content\":\"" + inputText + "\"}],"
                    + "\"temperature\": 0.3}";
  
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode == 200) {
      String response = http.getString();
      http.end();
      Serial.println(response);
  
      // Parse JSON response
      DynamicJsonDocument jsonDoc(1024);
      deserializeJson(jsonDoc, response);
      String outputText = jsonDoc["choices"][0]["message"]["content"];
      return outputText;
      // Serial.println(outputText);
    } else {
      http.end();
      Serial.printf("Error %i \n", httpResponseCode);
      return "<error>";
    }
  }
  