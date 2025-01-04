from multiprocessing import Process, Queue
from getter import getter_process
from worker import worker_process
from sender import sender_process

def main():
    work_queue = Queue()
    result_queue = Queue()

    # Start getter process
    getter = Process(target=getter_process, args=(work_queue,))
    getter.start()

    # Start worker processes
    num_workers = 3
    workers = []
    for _ in range(num_workers):
        worker = Process(target=worker_process, args=(work_queue, result_queue))
        worker.start()
        workers.append(worker)

    # Start sender process
    sender = Process(target=sender_process, args=(result_queue,))
    sender.start()

    # Wait for getter to finish
    getter.join()

    # Signal workers to stop
    for _ in workers:
        work_queue.put("STOP")
    for worker in workers:
        worker.join()

    # Signal sender to stop
    result_queue.put("STOP")
    sender.join()

if __name__ == "__main__":
    main()
