#include "my_baidu_ts.h"

#define TXT_DATA_LEN 1024 //STT txt len
#define ADC_DATA_LEN 1024*16 //read data len
const int data_json_len = ADC_DATA_LEN * 2 * 1.4;
 
// 1、修改百度语言技术的用户信息：https://console.bce.baidu.com/ai/?fromai=1#/ai/speech/app/list
const int STT_DEV_PID = 1537; //选填，输入法模型 1737-英语 1537-普通话(近场识别模型) 1936-普通话远程识别 1837-四川话 
const char *STT_TTS_CUID = "CoPY70iMA468o2r4PVLWmlLCruuYQd6G"; //用户唯一标识，用来区分用户，计算UV值。建议填写能区分用户的机器 MAC 地址或 IMEI 码，长度为60字符以内。
const char *STT_TTS_CLIENT_ID = "sOKyRkOGpc76TYCNvGcd2F1i"; //API Key
const char *STT_TTS_CLIENT_SECRET = "CoPY70iMA468o2r4PVLWmlLCruuYQd6G"; //Secret Key
 
String stt_tts_token;
String stt_tts_gainToken() {
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
 
void stt_tts_setup()
{
  stt_tts_token = stt_tts_gainToken();
  //Serial.println(stt_tts_token.c_str());
}
 
void stt_assembleJson(uint16_t *data, char *data_json)
{
  if (stt_tts_token == "")
  {
    stt_tts_setup();
  }
 
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
 
//待优化，合成成功，返回的Content-Type以“audio”开头, 
//合成出现错误，则会返回json文本，具体header信息为：Content-Type: application/json
int getInfoFromTtsResponse(String response, LLM_MSG_RSP_T *rsp)
{
  // Parse JSON response
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, response);
  rsp->err_msg = (String)jsonDoc["err_msg"];
  //rsp->err_msg = tmp1.c_str();
  rsp->err_no = jsonDoc["err_no"];;
  //Serial.println(rsp->err_msg);
  //Serial.println(rsp->err_no);
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
 
String sendToSTT(uint16_t *data)
{
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
 
String sendToTTS(String InputText, int *len) {
 
  InputText = urlEncode(InputText);//tex字段2次urlencode
  InputText = urlEncode(InputText);//百度为了更好地兼容，支持1次及2次urlencode， 其中2次urlencode可以覆盖全部的特殊字符。因而推荐传递tex 参数时做2次urlencode编码。
  HTTPClient http;
  char* tts_url = "https://tsn.baidu.com/text2audio"; // 百度语音合成的API URL
  http.begin(tts_url); // 初始化HTTP请求  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // 根据API要求添加HTTP头  application/x-www-form-urlencoded
 
  if (stt_tts_token == "")
  {
    stt_tts_setup();
  }
  String payload = String("tex=")+InputText.c_str()+String("&tok=")+stt_tts_token.c_str()+String("&cuid=")+STT_TTS_CUID+String("&ctp=1&lan=zh&spd=5&pit=1&vol=1&per=5&aue=4");  
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