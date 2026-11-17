#!/usr/bin/env python3
# DBUS 알림 클라이언트: 서버의 Notify 메소드를 호출해서 알림 메시지를 보낸다
import sys
import dbus

bus = dbus.SessionBus()
obj = bus.get_object("kr.lab2.Notifier", "/kr/lab2/Notifier")
iface = dbus.Interface(obj, "kr.lab2.Notifier")

msgs = sys.argv[1:] or ["안녕하세요, DBUS 알림입니다"]
for m in msgs:
    reply = iface.Notify(m)
    print("[클라이언트] 전송: %s -> 응답: %s" % (m, reply))
