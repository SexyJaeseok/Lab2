#!/usr/bin/env python3
# DBUS 알림 서버: Notify(메시지) 메소드를 제공하고, 받은 메시지를 출력한다
import dbus
import dbus.service
import dbus.mainloop.glib
from gi.repository import GLib

BUS_NAME = "kr.lab2.Notifier"
OBJ_PATH = "/kr/lab2/Notifier"
IFACE = "kr.lab2.Notifier"


class Notifier(dbus.service.Object):
    def __init__(self, bus):
        super().__init__(bus, OBJ_PATH)
        self.count = 0

    @dbus.service.method(IFACE, in_signature="s", out_signature="s")
    def Notify(self, msg):
        self.count += 1
        print("[서버] 알림 %d 수신: %s" % (self.count, msg), flush=True)
        # 받은 알림을 구독한 클라이언트에게 시그널로도 뿌려 준다
        self.Notified(msg)
        return "확인(%d)" % self.count

    @dbus.service.method(IFACE, in_signature="", out_signature="")
    def Quit(self):
        print("[서버] 종료 요청 수신", flush=True)
        loop.quit()

    @dbus.service.signal(IFACE, signature="s")
    def Notified(self, msg):
        pass


dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
bus = dbus.SessionBus()
name = dbus.service.BusName(BUS_NAME, bus)
server = Notifier(bus)
loop = GLib.MainLoop()
print("[서버] %s 준비 완료" % BUS_NAME, flush=True)
loop.run()
