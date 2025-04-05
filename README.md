# lib-stt<br>
Library for voice to text conversions<br>
<br><br>
**Example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v1/googlecloud/googlecloudv1 0<br>
[INFO] Created v1::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "Jak się dzisiaj czujesz", quality: 94%<br>
Speak now in english ...<br>
Text: "are you feeling well today", quality: 98%<br>
Speak now in polish again ...<br>
Text: "wszystko w porządku ale jutro być może będzie lepiej", quality: 94%<br>
[INFO] Released v1::gcloud stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v1/googlecloud/googlecloudv1 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v1::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.5t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Cześć jak się dzisiaj czujesz'/0.939331<br>
Text: "Cześć jak się dzisiaj czujesz", quality: 94%<br>
Speak now in english ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.5t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'actually I'm not feeling well today'/0.971372<br>
[DBG ] Speech detected for en-US/1<br>
Text: "actually I'm not feeling well today", quality: 97%<br>
Speak now in polish again ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.5t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Przykro mi stary Mam nadzieję że będzie jutro lepiej'/0.939331<br>
Text: "Przykro mi stary Mam nadzieję że będzie jutro lepiej", quality: 94%<br>
[INFO] Released v1::gcloud stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googleapi/googleapi 0<br>
[INFO] Created v2::gapi stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "Cześć co tam u ciebie", quality: 92%<br>
Speak now in english ...<br>
Text: "not bad not bad at all", quality: 97%<br>
Speak now in polish again ...<br>
Text: "raz dwa trzy", quality: 92%<br>
[INFO] Released v2::gapi stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googleapi/googleapi 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v2::gapi stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.flac silence -l 1 1 2.0% 1 2.0t 1.0% pad 0.3 0.2<br>
[DBG ] Returning transcript [text/confid]: 'Cześć co tam dobrego u ciebie słychać'/0.918985<br>
Text: "Cześć co tam dobrego u ciebie słychać", quality: 92%<br>
Speak now in english ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.flac silence -l 1 1 2.0% 1 2.0t 1.0% pad 0.3 0.2<br>
[DBG ] Returning transcript [text/confid]: 'not bad not bad at all Paul'/0.933015<br>
[DBG ] Speech detected for en-US/1<br>
Text: "not bad not bad at all Paul", quality: 93%<br>
Speak now in polish again ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.flac silence -l 1 1 2.0% 1 2.0t 1.0% pad 0.3 0.2<br>
[DBG ] Returning transcript [text/confid]: 'to spoko a u mnie mogłoby być trochę lepiej ale i tak nie narzekam dzięki'/0.904355<br>
Text: "to spoko a u mnie mogłoby być trochę lepiej ale i tak nie narzekam dzięki", quality: 90%<br>
[INFO] Released v2::gapi stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googlecloud/googlecloudv2 0<br>
[INFO] Created v2::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
Text: "Hejo jak ci się podobał wczorajszy mecz", quality: 94%<br>
Speak now in english ...<br>
Text: "I watched that but it was a waste of time", quality: 98%<br>
Speak now in polish again ...<br>
Text: "tak uważam i szkoda że nasi znowu przegrali", quality: 94%<br>
[INFO] Released v2::gcloud stt [langcode/langid]: pl-PL/0<br>
<br>
pi@raspberrypi:\~/git/lib-stt/build $ ./examples/v2/googlecloud/googlecloudv2 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created v2::gcloud stt [langcode/langid]: pl-PL/0<br>
Speak now in polish ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'Cześć Oglądałeś wczoraj w telewizji mecz o 20'/0.934335<br>
Text: "Cześć Oglądałeś wczoraj w telewizji mecz o 20", quality: 93%<br>
Speak now in english ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'yeah it was quite interesting but unfortunately I slept while watching it'/0.982026<br>
[DBG ] Speech detected for en-US/1<br>
Text: "yeah it was quite interesting but unfortunately I slept while watching it", quality: 98%<br>
Speak now in polish again ...<br>
[DBG ] Recording voice by: sox --no-show-progress --type alsa default --rate 16k --channels 1 audio/recording.wav silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2<br>
[DBG ] Uploaded audio to stt engine: audio/recording.wav<br>
[DBG ] Received results: 1<br>
[DBG ] Received alternatives: 1<br>
[DBG ] Returning transcript [text/confid]: 'rozumiem cię bracie ale nie martw się będzie z tego powtórka'/0.939331<br>
Text: "rozumiem cię bracie ale nie martw się będzie z tego powtórka", quality: 94%<br>
[INFO] Released v2::gcloud stt [langcode/langid]: pl-PL/0<br>
[DBG ] Removed directory: 'audio'<br>
