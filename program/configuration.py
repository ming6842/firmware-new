#!/usr/bin/env python

import sys
from PyQt4.QtGui import *

program_device_list = ["ST-Link", "OpenOCD", "Black magic probe"]

def form_setup():
	app = QApplication(sys.argv)
	main_form = QWidget()

	#Create the form
	main_form.setWindowTitle("Configuration");
	main_form.setFixedSize(250, 170)

	#Create the combo box component
	layout_combo = QVBoxLayout()
	label_flash = QLabel("Flash software:")
	label_debug = QLabel("Debug software:")
	combo_flash = QComboBox()
	combo_debug = QComboBox()
	layout_combo.addWidget(label_flash)
	layout_combo.addWidget(combo_flash)	
	layout_combo.addWidget(label_debug)
	layout_combo.addWidget(combo_debug)
	layout_combo.addStretch(1)
	
	for program_device_type in program_device_list:
		combo_flash.addItem(program_device_type)
		combo_debug.addItem(program_device_type)

	#Create the button component
	layout_button = QHBoxLayout()
	button_ok = QPushButton("OK")
	button_reset = QPushButton("Reset default")
	layout_button.addWidget(button_ok)
	layout_button.addWidget(button_reset)

	layout_form = QVBoxLayout()
	layout_form.addLayout(layout_combo)
	layout_form.addLayout(layout_button)	
	main_form.setLayout(layout_form)

	#Show up the form
	main_form.move(QApplication.desktop().screen().rect().center())
	main_form.show()
	app.exec_()

def run():
	form_setup()

run()
