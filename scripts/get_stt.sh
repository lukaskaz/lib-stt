#!/bin/bash

STTFILE=audio/recording.flac
CONFFILE=../conf/init.json
KEY=$(cat $CONFFILE|jq -r .stt.key)

echo "Please say your sentence"
sox -q -t alsa default -r 16000 -c 1 $STTFILE silence -l 1 1 2% 1 0.5 1% pad 0.5 0.5
#sox -q -r 16000 -t alsa default -c 1 $STTFILE silence -l 1 1 2% 1 0.5 1% pad 0.5 0.5
#sox -q -r 16000 -t alsa default -c 1 $STTFILE silence -l 1 1 2% 1 1.0 1% pad 0.2 0.5

#curl -X POST --data-binary @audio/recording.flac --header "Content-Type: audio/x-flac; rate=16000;" "https://www.google.com/speech-api/v2/recognize?output=json&lang=pl&key=#KEY#"

#echo 'curl -X POST --data-binary @audio/recording.flac --header "Content-Type: audio/x-flac; rate=16000;" "https://www.google.com/speech-api/v2/recognize?output=json&lang=pl&key=#KEY#"'
#echo curl -X POST --data-binary @audio/recording.flac --header "Content-Type: audio/x-flac; rate=16000;" "https://www.google.com/speech-api/v2/recognize?output=json&lang=pl&key=$KEY"

RESP=$(wget -q -U "Mozilla/5.0" --post-file $STTFILE --header "Content-Type: audio/x-flac; rate=16000" -O - "http://www.google.com/speech-api/v2/recognize?lang=pl&key=$KEY")

echo $RESP
#CONFID=$(echo $RESP|sed -r "s/^.*\{\"transcript\":\"([^\"]+).*confidence\":([^\}]+).*/\2/g")
CONFID=$(echo $RESP|jq .result[].alternative[0].confidence)
CONFID=$(echo "$CONFID * 100 / 1" | bc)

#RESP=$(echo $RESP|sed -r "s/^.*\{\"transcript\":\"([^\"]+).*confidence\":([^\}]+).*/\1/g")
RESP=$(echo $RESP|jq .result[].alternative[0].transcript)

echo "Text: $RESP"
echo "Confidence: $CONFID"



