# -*- coding: utf-8 -*-

import csv

# ��ԭʼ�ļ�����ȡ����
with open('iostat_pr.log', 'r') as file:
    lines = file.readlines()

# ����Ҫ��ȡ������
columns = ['%user', 'nice', '%system', '%iowait', '%steal', '%idle']

# ��ʼ��һ���յ������б�
data = []

# ����Ƿ��ҵ���"avg-cpu"
found_avg_cpu = False

# �����ļ���ÿһ��
for line in lines:
    # �����ǰ�а���"avg-cpu"�����������ΪTrue
    if 'avg-cpu' in line:
        found_avg_cpu = True
    # ����ҵ���"avg-cpu"�����ҵ�ǰ�в�����"avg-cpu"�����ȡ��ǰ�е�����
    elif found_avg_cpu:
        # �Կո�ָǰ�У�����ȡ�����Ǹ���Ȥ������
        values = line.split()[0:6]
        # ����ȡ��������ת��Ϊ������
        values = [float(value) for value in values]
        # ���������ӵ������б���
        data.append(values)
        # ���ñ��ΪFalse���Ա��´��ҵ�"avg-cpu"
        found_avg_cpu = False

# ����ȡ������д�� CSV �ļ�
with open('pr_cpu_output.csv', 'w') as csvfile:
    writer = csv.writer(csvfile)
    # д������
    writer.writerow(columns)
    # д������
    writer.writerows(data)

print("�����ѳɹ���ȡ��д�� output.csv �ļ���")
