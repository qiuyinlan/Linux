import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GLib
import datetime

class ClipboardManager:
    def __init__(self):
        self.clipboard = Gtk.Clipboard.get(Gdk.SELECTION_CLIPBOARD)
        self.history = []
        self.pinned_items = []
        
    def add_to_history(self, text):
        if text not in self.pinned_items:
            self.history.insert(0, text)
            
    def pin_item(self, text):
        if text not in self.pinned_items:
            self.pinned_items.append(text)
            
    def unpin_item(self, text):
        if text in self.pinned_items:
            self.pinned_items.remove(text)
            