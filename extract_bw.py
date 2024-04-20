# -*- coding: utf-8 -*-

import csv

# ����־�ļ���CSV�ļ�
with open('iostat_pr.log', 'r') as log_file, open('pr_bw_output.csv', 'w') as csvfile:
    # ����CSVд����������
    writer = csv.writer(csvfile)
    writer.writerow(['rMB/s', 'wMB/s', 'device_util'])

    # ���ж�ȡ��־�ļ�����������
    for line in log_file:
        # ʹ�ÿո�ÿ�����ݷָ���б�?
        if "nvme0n1" in line:
          data = line.split()
          device = data[0]
          # ��������Ƿ����"nvme0n1"
          if "nvme0n1" == device:
  
              # ����б����ȣ�ȷ����������?
              if len(data) >= 13:
                  # ��ȡ��5�У�����Ϊ4���͵�13�У�����Ϊ12��������
                  rKB_s = float(data[2])
                  rMB_s = rKB_s / 1000
                  wKB_s = float(data[8])
                  wMB_s = wKB_s / 1000
                  util = float(data[20])
  
                  # ����ȡ������д��CSV�ļ�
                  writer.writerow([rMB_s, wMB_s, util])
