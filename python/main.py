from multiprocessing import Process, Queue
from getter import getter_process
from worker import worker_process
from sender import sender_process


def main():
    work_queue = Queue()
    result_queue = Queue()

    getter = Process(target=getter_process, args=(work_queue,))
    getter.start()

    num_workers = 3
    workers = []

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

    result_queue.put("STOP")

    sender.join()


if __name__ == "__main__":
    main()
