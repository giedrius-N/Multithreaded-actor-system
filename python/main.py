# Processor: 11th Gen Intel(R) Core(TM) i5-11300H @ 3.10GHz (8 CPUs), ~3.1GHz
# - With one worker the program completed in 11.5282 seconds
# - With four workers the program completed in 4.40966 seconds

from multiprocessing import Process, Queue
from getter import getter_process
from worker import worker_process
from sender import sender_process
import time

def main():
    work_queue = Queue()
    result_queue = Queue()

    getter = Process(target=getter_process, args=(work_queue,))
    getter.start()

    num_workers = 4
    workers = []

    start_time = time.time()

    for _ in range(num_workers):
        worker = Process(target=worker_process, args=(work_queue, result_queue))
        worker.start()
        workers.append(worker)

    sender = Process(target=sender_process, args=(result_queue,))
    sender.start()

    getter.join()

    for _ in range(num_workers):
        work_queue.put("STOP")

    for worker in workers:
        worker.join()

    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Total processing time for workers: {elapsed_time:.2f} seconds")

    result_queue.put("STOP")

    sender.join()


if __name__ == "__main__":
    main()
