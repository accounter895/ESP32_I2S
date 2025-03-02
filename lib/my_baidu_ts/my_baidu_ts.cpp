#include "my_baidu_ts.h"

const int data_json_len = ADC_DATA_LEN * 2 * 1.4;

const int PER = 4;
const int SPD = 5;
const int PIT = 5;
const int VOL = 5;
const int AUE = 6;

const char *TTS_URL = "https://tsn.baidu.com/text2audio";
String url = TTS_URL;

int g_current_state = 0;
// 1、修改百度语言技术的用户信息：https://console.bce.baidu.com/ai/?fromai=1#/ai/speech/app/list
const int STT_DEV_PID = 1537; //选填，输入法模型 1737-英语 1537-普通话(近场识别模型) 1936-普通话远程识别 1837-四川话 
const char *STT_TTS_CUID = "2zY0nzwfwhEmZeueWu8FVpBIhCL0ugHA"; //用户唯一标识，用来区分用户，计算UV值。建议填写能区分用户的机器 MAC 地址或 IMEI 码，长度为60字符以内。
const char *STT_TTS_CLIENT_ID = "5yZRwHlE02QqFIrJoWo0hyuU"; //API Key
const char *STT_TTS_CLIENT_SECRET = "ihvjYMiFb3pjMwh8TL0ju1TFYPakJtTE"; //Secret Key
 
String stt_tts_token;
/**
 * @brief Function for gain token from Baidu STT&TTS.
 *
 * @param[in] void.
 * 
 * @return (String)token.
 * 
 **/
String stt_tts_gainToken() {
  Serial.println("stt_tts_gainToken run!!");
  HTTPClient stt_http;
  String token;
  String url = String("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=") + STT_TTS_CLIENT_ID + "&client_secret=" + STT_TTS_CLIENT_SECRET;
 
  stt_http.begin(url);
  int httpCode = stt_http.GET();
 
  if (httpCode > 0) {
    String payload = stt_http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    token = doc["access_token"].as<String>();
    Serial.println("stt:" + token);
  } else {
    Serial.println("Error on HTTP request for token");
  }
  stt_http.end();
  return token;
}
 
/**
 * @brief Function for Baidu STT&TTS setup.
 *
 * @param[in] arg : Pointer to the event sent by other function call.
 * 
 * @return void.
 * 
 **/
String stt_tts_setup()
{
  Serial.println("stt_tts_setup run!!");
  stt_tts_token = stt_tts_gainToken();
  //Serial.println(stt_tts_token.c_str());
  return stt_tts_token;
}

/**
 * @brief Function for JSON assemble.
 *
 * @param[in] arg : Pointer to the event sent by other function call.
 * 
 * @return void.
 * 
 **/
void stt_assembleJson(uint16_t *data, char *data_json)
{
  if (stt_tts_token == "")
  {
    stt_tts_setup();
  }
  Serial.println("assembleJson run!!");
  strcat(data_json, "{");
  strcat(data_json, "\"format\":\"pcm\",");
  strcat(data_json, "\"rate\":16000,");
  strcat(data_json, "\"dev_pid\":1537,");
  strcat(data_json, "\"channel\":1,");
  strcat(data_json, "\"cuid\":\""); strcat(data_json, STT_TTS_CUID); strcat(data_json, "\",");
  strcat(data_json, "\"token\":\""); strcat(data_json, stt_tts_token.c_str()); strcat(data_json, "\",");
  sprintf(data_json + strlen(data_json), "\"len\":%d,", ADC_DATA_LEN * sizeof(uint16_t));
  strcat(data_json, "\"speech\":\"");
  strcat(data_json, base64::encode((uint8_t *)data, ADC_DATA_LEN * sizeof(uint16_t)).c_str());
  //int tmp = base64::decode((char *)adc_data, adc_data_len, data_json);
  strcat(data_json, "\"");
  strcat(data_json, "}");
  //Serial.println(data_json);
  return;
}

/**
 * @brief Function for parse JSON response and get text.
 *
 * @param[in] response : The JSON response form STT.
 * 
 * @return (String)results form STT.
 * 
 **/
String getTextFromResponse(String response)
{
  // Parse JSON response
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, response);
  String outputText = jsonDoc["result"];
  int len = strlen(outputText.c_str());
  String output = outputText.substring(2, len-2);
  //Serial.println(output);
  return output;
}

/**
 * @brief Function for 处理tts相应信息.
 *
 * @param[in] response : The JSON response form TTS.
 * @param[out] rsp : tts是否成功的标志.
 * 
 * @return (int) 0:成功，-1:解析失败，-2:缺少必要的键.
 * 
 **/
//待优化，合成成功，返回的Content-Type以“audio”开头,    **已优化**
//合成出现错误，则会返回json文本，具体header信息为：Content-Type: application/json
int getInfoFromTtsResponse(String response, LLM_MSG_RSP_T *rsp) {
  // Parse JSON response
  DynamicJsonDocument jsonDoc(1024);
  DeserializationError error = deserializeJson(jsonDoc, response);

  if (error) {
    Serial.println("Failed to parse JSON response");
    rsp->err_msg = "Failed to parse JSON response";
    rsp->err_no = -1; // 使用一个特定的错误码表示解析失败
    return -1;
  }

  if (!jsonDoc.containsKey("err_msg") || !jsonDoc.containsKey("err_no")) {
    Serial.println("JSON response missing 'err_msg' or 'err_no'");
    rsp->err_msg = "JSON response missing 'err_msg' or 'err_no'";
    rsp->err_no = -2; // 使用一个特定的错误码表示缺少必要的键
    return -2;
  }

  rsp->err_msg = jsonDoc["err_msg"].as<String>();
  rsp->err_no = jsonDoc["err_no"].as<int>();

  // Serial.println(rsp->err_msg);
  // Serial.println(rsp->err_no);

  return rsp->err_no;
} 

HTTPClient http_client_stt;
String sendToSTT_test(uint16_t *data)
{
  char *data_json = (char *)malloc(data_json_len*sizeof(char));
  memset(data_json, '\0', data_json_len*sizeof(char));
  stt_assembleJson(data, data_json);
  int httpCode = http_client_stt.POST(data_json);
  free(data_json);
}
 
/**
 * @brief Function for 把inmp4411的音频数据转换成json格式并发送到TTS服务器,获得文本.
 *
 * @param[in] *data : 音频数据.
 * 
 * 
 * @return (String)语音输入之后stt返回的文本数据.
 * 
 **/
String sendToSTT(uint16_t *data)
{
  Serial.println("sendToSTT run!!");
  HTTPClient http_client_stt;
  http_client_stt.begin("http://vop.baidu.com/server_api");//短语音识别请求地址: 标准版http://vop.baidu.com/server_api, 极速版https://vop.baidu.com/pro_api
  http_client_stt.addHeader("Content-Type", "application/json");
 
  char *data_json = (char *)malloc(data_json_len*sizeof(char));
  memset(data_json, '\0', data_json_len*sizeof(char));
  stt_assembleJson(data, data_json);
  int httpCode = http_client_stt.POST(data_json);
  free(data_json);
 
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String response = http_client_stt.getString();
      //Serial.println(response);
      String outputText = getTextFromResponse(response);
      http_client_stt.end();
      return outputText;
    }
  } else {
    Serial.printf("[HTTP] POST failed, error: %s\n", http_client_stt.errorToString(httpCode).c_str());
    http_client_stt.end();
    return String("响应失败请重新获取!");
  }
}

/*String sendToTTS(String InputText, int *len) {
 
  InputText = urlEncode(InputText);//tex字段2次urlencode
  InputText = urlEncode(InputText);//百度为了更好地兼容，支持1次及2次urlencode， 其中2次urlencode可以覆盖全部的特殊字符。因而推荐传递tex 参数时做2次urlencode编码。
  HTTPClient http;
  char* tts_url = "http://tsn.baidu.com/text2audio"; // 百度语音合成的API URL
  http.begin(tts_url); // 初始化HTTP请求  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // 根据API要求添加HTTP头  application/x-www-form-urlencoded
 
  if (stt_tts_token == "")
  {
    stt_tts_setup();
  }
  String payload = String("tex=")+InputText.c_str()+String("&tok=24.4645a3816cf83f36fcb132fcc75b7ebc.2592000.1743305804.282335-117723335")+String("&cuid=")+STT_TTS_CUID+String("&ctp=1&lan=zh&spd=5&pit=5&vol=5&per=4&aue=6");  
  //Serial.println(payload);  
  
  String outputText;
  int httpCode = http.POST(payload); // 发送POST请求  
  if (httpCode == HTTP_CODE_OK) {  
    String response = http.getString(); // 获取响应体  
    //Serial.println(response);
    LLM_MSG_RSP_T rsp_info;
    if (getInfoFromTtsResponse(response, &rsp_info))
    {
        Serial.println(response);
        outputText = rsp_info.err_msg;
        return outputText;
    }
    *len = http.getSize();
    //Serial.println(*len);
    http.end(); // 结束HTTP请求  
    return response;
  } else {  
    Serial.println("Error in the HTTP request");
    outputText = String("Error in the HTTP request");
  }  
  
  http.end(); // 结束HTTP请求  
  return outputText;
}
*/

/**
 * @brief Function for 把文本用tts转换为语音.
 *
 * @param[in] void.
 * 
 * @return (String)tts是否成功的字符串.
 * 
 **/
String tts_get(String encodedText) {
  url = TTS_URL;            //重置url
  const char *headerKeys[] = { "Content-Type", "Content-Length" };
  // 5、修改百度语音助手的token
  url += "?tok=24.dcb0788463590edacd07841f35d2bb5f.2592000.1743050163.282335-117723335";
  url += "&tex=" + encodedText;
  url += "&per=" + String(PER);
  url += "&spd=" + String(SPD);
  url += "&pit=" + String(PIT);
  url += "&vol=" + String(VOL);
  url += "&aue=" + String(AUE);
  url += "&cuid=esp32s3";
  url += "&lan=zh";
  url += "&ctp=1";

  HTTPClient http;

  Serial.print("URL: ");
  Serial.println(url);

  http.begin(url);
  http.collectHeaders(headerKeys, 2);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    if (httpResponseCode == HTTP_CODE_OK) {
      Serial.print("Content-Type = ");
      Serial.println(http.header("Content-Type"));
      String contentType = http.header("Content-Type");
      if (contentType.startsWith("audio")) {
        Serial.println("合成成功，返回的是音频文件");
        // 处理音频文件，保存到SD卡或者播放
      } else if (contentType.equals("application/json")) {
        Serial.println("合成出现错误,返回的是JSON文本");
        // 处理错误信息，根据需要进行相应的处理
      } else {
        Serial.println("未知的Content-Type");
        // 可以添加相应的处理逻辑
      }
    } else {
      Serial.println("Failed to receive audio file");
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return url;
}

/**
 * @brief Function for 播放语音.
 *
 * @param[in] input : 文本数据.
 * 
 * @return void.
 * 
 **/
void audio_play_by_text(String input)
{
    g_current_state |= LLM_PLAY_AUDIO_FLAG;
    int len = 0, i = 0, tmp = 0;
    String Output;
    uint16_t *wr_data = NULL;
    size_t bytes_write = 0;
 
    Output = sendToTTS(input, &len);
    wr_data = (uint16_t *)malloc(1024*16*sizeof(uint16_t));//1s
    for (i = 0; i < len; i+=(1024*16*sizeof(uint16_t)))
    {
      memset(wr_data, '\0', 1024*16*sizeof(uint16_t));
      tmp = len - i;
      if (len - i > 1024*16*sizeof(uint16_t))
        tmp = 1024*16*sizeof(uint16_t);
    
      memcpy(wr_data, Output.c_str()+i, tmp);
      esp_err_t result = i2s_write(I2S_NUM_1, wr_data, tmp, &bytes_write, portMAX_DELAY);
    }
    free(wr_data);
 
    g_current_state &= ~LLM_PLAY_AUDIO_FLAG;
    return;
}
