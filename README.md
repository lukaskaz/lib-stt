# lib-stt<br>
Library for voice to text conversions<br>
<br><br>
**Example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v1/googlecloud/googlecloudv1 0<br>
[INFO] Created v1::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "Jak się dzisiaj czujesz", quality: 94%<br>
Speak now in english ...<br>
Text: "are you feeling well today", quality: 84%<br>
Speak now in polish again ...<br>
Text: "wszystko w porządku ale jutro może być lepiej", quality: 94%<br>
[INFO] Released v1::gcloud stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v1/googlecloud/googlecloudv1 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v1::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Cześć jak się dzisiaj czujesz'/0.939331<br>
Text: "Cześć jak się dzisiaj czujesz", quality: 94%<br>
Speak now in english ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'actually I'm not feeling well today'/0.979290<br>
[DBG ] Speech detected for en-US/1<br>
Text: "actually I'm not feeling well today", quality: 98%<br>
Speak now in polish again ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Przykro mi stary Mam nadzieję że będzie lepiej'/0.939331<br>
Text: "Przykro mi stary Mam nadzieję że będzie lepiej", quality: 94%<br>
[INFO] Released v1::gcloud stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googleapi/googleapi 0<br>
[INFO] Created v2::gapi stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "Cześć co tam u ciebie", quality: 92%<br>
Speak now in english ...<br>
Text: "not bad not bad at all Paul", quality: 92%<br>
Speak now in polish again ...<br>
Text: "to spoko a u mnie mogło być trochę lepiej ale i tak nie narzekam", quality: 91%<br>
[INFO] Released v2::gapi stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googleapi/googleapi 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v2::gapi stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Returning transcript [text/confid]: 'Cześć co tam u ciebie'/0.918985<br>
Text: "Cześć co tam u ciebie", quality: 92%<br>
Speak now in english ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Returning transcript [text/confid]: 'not bad not bad at all Paul'/0.959956<br>
[DBG ] Speech detected for en-US/1<br>
Text: "not bad not bad at all Paul", quality: 96%<br>
Speak now in polish again ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Returning transcript [text/confid]: 'to spoko a u mnie mogłoby być troszeczkę lepiej ale i tak nie narzekam dzięki'/0.918985<br>
Text: "to spoko a u mnie mogłoby być troszeczkę lepiej ale i tak nie narzekam dzięki", quality: 92%<br>
[INFO] Released v2::gapi stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googlecloud/googlecloudv2 0<br>
[INFO] Created v2::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "jak Ci się podobał wczorajszy mecz", quality: 94%<br>
Speak now in english ...<br>
Text: "I watched that but it was a waste of time", quality: 95%<br>
Speak now in polish again ...<br>
Text: "też tak uważam i szkoda że nasi znowu przegrali", quality: 94%<br>
[INFO] Released v2::gcloud stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googlecloud/googlecloudv2 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v2::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Cześć jak Ci się podobał wczorajszy mecz w telewizji o 20'/0.915017<br>
Text: "Cześć jak Ci się podobał wczorajszy mecz w telewizji o 20", quality: 92%<br>
Speak now in english ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'yeah it was quite interesting but I slept while watching it unfortunately'/0.972338<br>
[DBG ] Speech detected for en-US/1<br>
Text: "yeah it was quite interesting but I slept while watching it unfortunately", quality: 97%<br>
Speak now in polish again ...<br>
[DBG ] Waiting for speech to process<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'rozumiem cię stary ale nie martw się będzie powtórka'/0.939331<br>
Text: "rozumiem cię stary ale nie martw się będzie powtórka", quality: 94%<br>
[INFO] Released v2::gcloud stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
