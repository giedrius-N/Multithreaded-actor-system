from multiprocessing import Queue

def sender_process(result_queue: Queue) -> None:
    server_address = '127.0.0.1'
    server_port = 3491
    accumulated_results = []
    
    while True:
        try:
            # Get a single filtered result from the result queue
            result = result_queue.get(timeout=5)  # Timeout of 5 seconds
            if result == "STOP":
                print("Sender stopping.")
                break
            
            # Accumulate the result
            accumulated_results.append(result)
            #print(f"Accumulated result (server not connected): {result}")
                
        except Exception as e:
            print("No more results to process.")
            break
    
    if accumulated_results:
        print("Final results collected")
        #print(f"Final accumulated results (server not connected): {accumulated_results}")