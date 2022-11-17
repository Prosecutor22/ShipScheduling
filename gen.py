import random

num_test = 7
berth_length = [500]
break_num = [20]
num_vessel = [200]
max_arrival_time = [600]
processing_max = 50
weight_max = 10

for i in range(2,3):
    name_file = f"input{i}.txt"
    with open(name_file,'w') as f:
        f.write("%  Berth length\n")   
        j = i - 2
        f.write(str(berth_length[j]) + '\n')
        f.write('\n')
        berth = random.sample(range(10,berth_length[j]),break_num[j])
        berth = sorted(berth)
        f.write("% Berth breaks\n")
        for ele in berth:
            f.write(str(ele) + '\n')
        f.write('\n')
        max_dis = 0
        for k in range(len(berth)):
            if k == 0:
                max_dis = berth[k]
                continue
            max_dis = max(max_dis,berth[k] - berth[k-1])
        max_dis = max(max_dis,berth_length[j] - berth[len(berth) - 1])
        f.write("% Vessel index, size, arrival time, processing time, weight (=1 as default) \n")
        for k in range(1,num_vessel[j]+1):
            arrival_t = random.randint(0,max_arrival_time[j])
            size = random.randint(1,max_dis) 
            processing_time = random.randint(1,processing_max)
            weight = random.randint(1,weight_max)
            f.write(f"{k} {size} {arrival_t} {processing_time} {weight}\n")
    
        
    