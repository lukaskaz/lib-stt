#!/bin/bash

STTFILE=audio/recording.flac
CONFFILE=../conf/init.json
KEY=$(cat $CONFFILE|jq -r .stt.key)
STARTDURATION=1
STARTTHRESHOLD=2.2
THRESHOLDSTEP=0.2

REDFONT="\033[0;31m"
GREENFONT="\033[0;32m"
YELLOWFONT="\033[0;33m"
NOCOLORFONT="\033[0m"

declare -a VOICEINPUTS=( \
  "Ten ptak wpadł w krzak" \
  "Kto to Piotr Piotrkowski" \
  "Czy robot może spać" \
)

function _getsttresult()
{
	local COMMAND=$1
	local INPUT=$2
	local -n RESULT=$3

	while (true); do 
		# echo $COMMAND
		INPUT=$(echo "$INPUT" | tr '[:lower:]' '[:upper:]')
		echo "Say: $INPUT"
		$COMMAND
		RESULT=$(wget -q -U "Mozilla/5.0" --post-file $STTFILE --header \
			"Content-Type: audio/x-flac; rate=16000" -O - \
	       		"http://www.google.com/speech-api/v2/recognize?lang=pl&key=$KEY")
		CONFID=$(echo $RESULT | jq .result[].alternative[0].confidence)
		CONFID=$(echo "$CONFID * 100 / 1" | bc)
		RESULT=$(echo $RESP | jq .result[].alternative[0].transcript)
		if [ -z "$RESP" ]; then
			echo ">> [WARN] Empty resp, repeating..."
			continue
		fi

		# echo "Text: $RESULT @ ${CONFID}%"
		break
	done
	return 0
}

trap 'echo "Cleanup and exit!"; killall -s KILL sox; exit 0' SIGINT SIGSTOP

RET=1
while [ $RET -ne 0 ]; do
	CMD="sox --no-show-progress --type alsa default --rate 16k --channels 1 \
$STTFILE silence -l 1 1 ${STARTTHRESHOLD}% 1 1.0t 1.0% pad 0.3 0.2"
	for INPUT in "${VOICEINPUTS[@]}"; do
		echo "Starting with duration: $STARTDURATION, and threshold: ${STARTTHRESHOLD}%"
		RESP=""
		_getsttresult "$CMD" "$INPUT" RESP
		RESP=$(echo "$RESP" | tr '[:upper:]' '[:lower:]'| tr -d '"')
		INPUT=$(echo "$INPUT" | tr '[:upper:]' '[:lower:]')
		echo "input: '$INPUT'"
		echo "resp:  '$RESP'"
		if [ -n "$RESP" -a "$RESP" == "$INPUT" ]; then
			echo -e "[${GREENFONT}OK${NOCOLORFONT}] We have a match here\n"
			RET=0
		else
			STARTTHRESHOLD=$(echo "$STARTTHRESHOLD - $THRESHOLDSTEP" | bc)
			if [ $(echo "$STARTTHRESHOLD <= 0.0" | bc -l) -eq 1 ]; then
				echo -e "[${REDFONT}FAILED${NOCOLORFONT}] Cannot find proper settings"
				exit 1
			else
				echo -e "[${YELLOWFONT}FAIL${NOCOLORFONT}] No match, changing parameters\n"
				RET=1
				break
			fi
		fi
	done
done
echo "-----"
echo -e "[${GREENFONT}SUCCESS${NOCOLORFONT}] Suggested threshold: ${STARTTHRESHOLD}%\a\a"
echo "Full command: $CMD"
echo "-----"
exit 0

