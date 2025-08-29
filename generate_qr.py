#!/usr/bin/env python3
"""
QR 코드 생성 스크립트
qrcode 라이브러리 설치 필요: pip install qrcode[pil]
"""

import qrcode

# QR 코드 생성
qr = qrcode.QRCode(
    version=1,
    error_correction=qrcode.constants.ERROR_CORRECT_L,
    box_size=10,
    border=4,
)

# 데이터 추가
qr.add_data('123456789')
qr.make(fit=True)

# 이미지 생성
img = qr.make_image(fill_color="black", back_color="white")

# 이미지 저장
img.save("qrcode_123456789.png")
print("QR 코드가 생성되었습니다: qrcode_123456789.png")