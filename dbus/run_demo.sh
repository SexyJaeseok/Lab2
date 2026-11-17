#!/bin/bash
# 서버를 백그라운드로 띄우고 클라이언트가 알림 메시지 2개를 보낸 뒤 서버를 종료한다
# 사용법: dbus-run-session -- bash run_demo.sh
python3 notify_server.py &
sleep 1.5
python3 notify_client.py "회의가 10분 후에 시작됩니다" "과제 제출 마감 임박"
sleep 0.5
gdbus call --session -d kr.lab2.Notifier -o /kr/lab2/Notifier -m kr.lab2.Notifier.Quit > /dev/null
wait
