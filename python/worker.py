from multiprocessing import Queue

def process_item(item) -> int:
    # Simulate computation
    for i in range(10000000):
        pass

    return item["sunnyDays"] ** 2

def worker_process(work_queue: Queue, result_queue: Queue) -> None:
    while True:
        item = work_queue.get()
        if item == "STOP":
            print("Worker stopping.")
            break

        sunnyDaysIndex = process_item(item)

        if sunnyDaysIndex >= 62500:  # Filter to only include items with sunnyDays >= 250
            result_queue.put(item)
